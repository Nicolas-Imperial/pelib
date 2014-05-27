#include <Record.hpp>
#include <Taskgraph.hpp>
#include <Architecture.hpp>
#include <Schedule.hpp>

#ifndef PELIB_STREAMINGAPP
#define PELIB_STREAMINGAPP

namespace pelib
{
	class StreamingApp : public Record
	{
		public:				
			StreamingApp();
			StreamingApp(const StreamingApp &rhs);
			virtual	~StreamingApp();

			virtual const Schedule&
			getSchedule() const;

			virtual const Taskgraph&
			getTaskgraph() const;

			virtual const Architecture&
			getArchitecture() const;

			virtual StreamingApp&
			operator=(const StreamingApp &rhs);
		protected:
			/*
			Schedule schedule;
			Taskgraph taskgraph;
			Architecture architecture;
			*/

		private:
	};
}

#endif
