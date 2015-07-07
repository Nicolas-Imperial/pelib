#include <string>

#include <pelib/Data.hpp>

#ifndef PELIB_ALGEBRADATA
#define PELIB_ALGEBRADATA

namespace pelib
{
	class AlgebraData : public Data
	{
		public:
			AlgebraData(const std::string);
			AlgebraData(const AlgebraData&);

			virtual	const std::string&
			getName() const;

			virtual void
			setName(const std::string);

			virtual AlgebraData* clone() const = 0;
		protected:
			std::string name;
	};
}

#endif
