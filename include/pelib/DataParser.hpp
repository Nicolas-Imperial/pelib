#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>
#include <pelib/Algebra.hpp>
#include <pelib/Data.hpp>

#ifndef PELIB_DATAPARSER
#define PELIB_DATAPARSER

namespace pelib
{
	class DataParser
	{
		public:
			virtual ~DataParser();
			virtual
			DataParser*
			clone() const = 0;

		protected:
		private:
	};
}

#endif
