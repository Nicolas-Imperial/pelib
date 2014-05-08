#include <AmplDataParser.hpp>
#include <AmplDataOutput.hpp>

#ifndef PELIB_AMPLINPUTDATA
#define PELIB_AMPLINPUTDATA

namespace pelib
{
	class AmplInputData: public AmplDataOutput, public AmplDataParser
	{
		public:
			AmplInputData(bool strict) : AmplDataParser(strict)	{}
	  virtual ~AmplInputData(){}
			virtual
			AmplInputData*
			clone() const = 0;
		protected:
		private:		
	};
}

#endif
