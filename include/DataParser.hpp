#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <ParseException.hpp>
#include <NoDecimalFloatException.hpp>
#include <Algebra.hpp>
#include <Data.hpp>

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
