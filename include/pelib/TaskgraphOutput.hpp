#include <pelib/Taskgraph.hpp>
#include <pelib/Platform.hpp>
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_TASKGRAPHOUTPUT
#define PELIB_TASKGRAPHOUTPUT

namespace pelib
{
	class TaskgraphOutput : public RecordOutput
	{
		public:	
			virtual TaskgraphOutput* clone() const = 0;
			virtual	void dump(std::ostream &os, const Taskgraph &data, const Platform &arch) const = 0;
			virtual	void dump(std::ostream &os, const Taskgraph *data, const Platform *arch) const = 0;

		protected:
		private:		
	};
}

#endif
