#include <string>

#include <Data.hpp>

#ifndef PELIB_ALGEBRADATA
#define PELIB_ALGEBRADATA

namespace pelib
{
	class AlgebraData : public Data
	{
		public:
			AlgebraData(const std::string);
			AlgebraData(const AlgebraData&);

			virtual AlgebraData* clone() const = 0;
	};
}

#endif
