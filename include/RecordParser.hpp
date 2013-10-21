#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <DataParser.hpp>
#include <Record.hpp>

#ifndef PELIB_RECORDPARSER
#define PELIB_RECORDPARSER

namespace pelib
{
	class RecordParser
	{
		public:
			virtual
			~RecordParser();
			
			virtual
			Record
			parse(std::istream &ampl_data, bool strict = 0) = 0;

		protected:
		private:
	};
}

#endif