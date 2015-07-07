#include <string>

#include <pelib/AmplDataParser.hpp>

#ifndef PELIB_AMPLOUTPUTDATAPARSER
#define PELIB_AMPLOUTPUTDATAPARSER

namespace pelib
{
	class AmplOutputDataParser: public AmplDataParser
	{
		public:
			virtual
			AmplOutputDataParser*
			clone() const = 0;

		protected:
		private:
	};
}

#endif
