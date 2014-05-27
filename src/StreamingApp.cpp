#include <StreamingApp.hpp>

namespace pelib
{	
	StreamingApp::StreamingApp() { /* Do nothing */ }
	StreamingApp::~StreamingApp() { /* Do nothing */ }

	StreamingApp::StreamingApp(const StreamingApp &rhs)
	{
		*this = rhs;
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
