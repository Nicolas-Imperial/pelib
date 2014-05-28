#include <RecordParser.hpp>
#include <StreamingApp.hpp>

#ifndef PELIB_STREAMINGAPPPARSER
#define PELIB_STREAMINGAPPPARSER

namespace pelib
{
	class StreamingAppParser : public RecordParser
	{
		public:
			virtual
			StreamingApp
			parse(std::istream &taskgraph, std::istream &architecture, std::istream &schedule) const = 0;
	};
}

#endif
