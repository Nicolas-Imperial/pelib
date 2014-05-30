#include <Taskgraph.hpp>
#include <StreamingAppDataParser.hpp>

#ifndef PELIB_TASKGRAPHPARSER
#define PELIB_TASKGRAPHPARSER

namespace pelib
{
	class TaskgraphParser : public StreamingAppDataParser
	{
		public:
			virtual
			Taskgraph* parse(std::istream &in) const = 0;

			virtual TaskgraphParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
