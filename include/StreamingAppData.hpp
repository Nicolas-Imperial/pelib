#include <string>

#include <Data.hpp>
#include <Algebra.hpp>

#ifndef PELIB_STREAMINGAPPDATA
#define PELIB_STREAMINGAPPDATA

namespace pelib
{
	class StreamingAppData : public Data
	{
		public:
			virtual StreamingAppData* clone() const = 0;
			virtual Algebra buildAlgebra() const = 0;
	};
}

#endif
