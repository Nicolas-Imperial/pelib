#include <Taskgraph.hpp>
#include <Architecture.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_TASKGRAPHOUTPUT
#define PELIB_TASKGRAPHOUTPUT

namespace pelib
{
	class TaskgraphOutput : public StreamingAppDataOutput
	{
		public:	
			virtual TaskgraphOutput* clone() const = 0;
			virtual	void dump(std::ostream &os, const StreamingAppData &data, const Architecture &arch) const = 0;
			virtual	void dump(std::ostream &os, const StreamingAppData *data, const Architecture *arch) const = 0;

		protected:
		private:		
	};
}

#endif
