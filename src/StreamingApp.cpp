#include <StreamingApp.hpp>

namespace pelib
{	
	StreamingApp::StreamingApp() { /* Do nothing */ }
	StreamingApp::~StreamingApp() { /* Do nothing */ }

	StreamingApp::StreamingApp(const StreamingApp &rhs)
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
			/*
			this->schedule = rhs.getSchedule();
			this->architecture = rhs.getArchitecture();
			this->taskgraph = rhs.getTaskgraph();
			*/
		}
		
		return *this;
	}
}
