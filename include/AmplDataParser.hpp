#include <string>

#include <DataParser.hpp>

#ifndef PELIB_AMPLDATAPARSER
#define PELIB_AMPLDATAPARSER

namespace pelib
{
	class AmplDataParser: public DataParser
	{
		public:
			AmplDataParser(bool strict) : DataParser(strict) {}
			
			virtual
			AmplDataParser*
			clone() const = 0;
			
		protected:
		private:
	};
}

#endif
