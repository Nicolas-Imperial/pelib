#include <pelib/Taskgraph.hpp>

#include <pelib/TaskgraphParser.hpp>

#ifndef PELIB_GRAPHMLPARSER_H
#define PELIB_GRAPHMLPARSER_H

namespace pelib
{
	class GraphMLParser : public TaskgraphParser
	{
		public:
			virtual Taskgraph* parse(std::istream &is) const = 0;
	};
}

#endif
