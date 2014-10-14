#include <Record.hpp>
#include <Taskgraph.hpp>
#include <Platform.hpp>
#include <Schedule.hpp>
#include <Algebra.hpp>

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

			virtual const Platform&
			getPlatform() const;

			virtual void
			insert(StreamingAppData* data);

			virtual Algebra
			buildAlgebra() const;

			virtual StreamingApp&
			operator=(const StreamingApp &rhs);
		protected:
			Schedule schedule;
			Taskgraph taskgraph;
			Platform platform;

		private:
	};
}

#endif
