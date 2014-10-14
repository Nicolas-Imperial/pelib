#include <Taskgraph.hpp>
#include <Platform.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_TASKGRAPHOUTPUT
#define PELIB_TASKGRAPHOUTPUT

namespace pelib
{
	class TaskgraphOutput : public StreamingAppDataOutput
	{
		public:	
			virtual TaskgraphOutput* clone() const = 0;
			virtual	void dump(std::ostream &os, const StreamingAppData &data, const Platform &arch) const = 0;
			virtual	void dump(std::ostream &os, const StreamingAppData *data, const Platform *arch) const = 0;

		protected:
		private:		
	};
}

#endif
