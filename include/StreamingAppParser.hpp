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
			parse(std::istream &data) = 0;
	};
}

#endif
