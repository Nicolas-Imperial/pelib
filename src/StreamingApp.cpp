#include <StreamingApp.hpp>
#include <CastException.hpp>

namespace pelib
{	
	StreamingApp::StreamingApp() : schedule("streaming", "streaming") { /* Do nothing */ }
	StreamingApp::~StreamingApp() { /* Do nothing */ }

	StreamingApp::StreamingApp(const StreamingApp &rhs) : schedule(rhs.getSchedule().getName(), rhs.getSchedule().getAppName()) 
	{
		*this = rhs;
	}

	const Schedule&
	StreamingApp::getSchedule() const
	{
		return this->schedule;
	}

	const Taskgraph&
	StreamingApp::getTaskgraph() const
	{
		return this->taskgraph;
	}

	const Platform&
	StreamingApp::getPlatform() const
	{
		return this->platform;
	}

	StreamingApp&
	StreamingApp::operator=(const StreamingApp &rhs)
	{
		if(this != &rhs)
		{
			this->schedule = rhs.getSchedule();
			this->platform = rhs.getPlatform();
			this->taskgraph = rhs.getTaskgraph();
		}
		
		return *this;
	}

	void
	StreamingApp::insert(StreamingAppData* data)
	{
		const Taskgraph *tg = dynamic_cast<const Taskgraph* >(data);
		if(tg == NULL)
		{
			const Platform *arch = dynamic_cast<const Platform* >(data);
			if(arch == NULL)
			{
				const Schedule *sched = dynamic_cast<const Schedule* >(data);
				if(sched == NULL)
				{
					throw CastException("Parameter \"data\" was not of type \"Taskgraph*\", \"Platform*\" or \"Schedule*\".");
				}
				else
				{
					this->schedule = *(Schedule*)data;
				}
			}
			else
			{
				this->platform = *arch;
			}
		}
		else
		{
			this->taskgraph = *(Taskgraph*)data;
		}
	}

	Algebra
	StreamingApp::buildAlgebra() const
	{
		Algebra ret = this->getTaskgraph().buildAlgebra(this->getPlatform());
		ret = ret.merge(this->getPlatform().buildAlgebra());
		ret = ret.merge(this->getSchedule().buildAlgebra());

		return ret;
	}
}
