#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <RecordParser.hpp>
#include <Algebra.hpp>

#ifndef PELIB_ALGEBRAPARSER
#define PELIB_ALGEBRAPARSER

namespace pelib
{
	class AlgebraParser : public RecordParser
	{
		public:
			virtual
			Algebra
			parse(std::istream &data) const = 0;
	};
}

#endif
