#include <StreamingApp.hpp>
#include <CastException.hpp>

namespace pelib
{	
	StreamingApp::StreamingApp() : schedule("streaming", "streaming") { /* Do nothing */ }
	StreamingApp::~StreamingApp() { /* Do nothing */ }

	StreamingApp::StreamingApp(const StreamingApp &rhs) : schedule(rhs.getSchedule().getName(), rhs.getSchedule().getAUTName()) 
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

	const Architecture&
	StreamingApp::getArchitecture() const
	{
		return this->architecture;
	}

	StreamingApp&
	StreamingApp::operator=(const StreamingApp &rhs)
	{
		if(this != &rhs)
		{
			this->schedule = rhs.getSchedule();
			this->architecture = rhs.getArchitecture();
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
			const Architecture *arch = dynamic_cast<const Architecture* >(data);
			if(arch == NULL)
			{
				const Schedule *sched = dynamic_cast<const Schedule* >(data);
				if(sched == NULL)
				{
					throw CastException("Parameter \"data\" was not of type \"Taskgraph*\", \"Architecture*\" or \"Schedule*\".");
				}
				else
				{
					this->schedule = *(Schedule*)data;
				}
			}
			else
			{
				this->architecture = *arch;
			}
		}
		else
		{
			this->taskgraph = *(Taskgraph*)data;
		}
	}
}
