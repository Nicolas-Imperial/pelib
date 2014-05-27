#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <ParseException.hpp>
#include <NoDecimalFloatException.hpp>
#include <StreamingApp.hpp>
#include <StreamingAppData.hpp>
#include <DataParser.hpp>

#ifndef PELIB_STREAMINGAPPDATAPARSER
#define PELIB_STREAMINGAPPDATAPARSER

namespace pelib
{
	class StreamingAppDataParser : public DataParser
	{
		public:
			virtual
			StreamingAppData*
			parse(std::istream &in) = 0;

			virtual
			StreamingAppDataParser*
			clone() const = 0;
		protected:
		private:
	};
}

#endif
