#include <pelib/Taskgraph.hpp>
#include <pelib/DataParser.hpp>

#ifndef PELIB_TASKGRAPHPARSER
#define PELIB_TASKGRAPHPARSER

namespace pelib
{
	class TaskgraphParser : public DataParser
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
