#include <string>

#include <pelib/AmplDataParser.hpp>

#ifndef PELIB_AMPLINPUTDATAPARSER
#define PELIB_AMPLINPUTDATAPARSER

namespace pelib
{
	class AmplInputDataParser: public AmplDataParser
	{
		public:
			virtual
			AmplInputDataParser*
			clone() const = 0;

		protected:
		private:
	};
}

#endif
