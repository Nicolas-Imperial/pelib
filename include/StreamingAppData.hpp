#include <string>

#include <Data.hpp>

#ifndef PELIB_STREAMINGAPPDATA
#define PELIB_STREAMINGAPPDATA

namespace pelib
{
	class StreamingAppData : public Data
	{
		public:
			virtual StreamingAppData* clone() const = 0;
	};
}

#endif
