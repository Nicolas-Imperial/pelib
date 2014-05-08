#include <AmplDataParser.hpp>
#include <AmplDataOutput.hpp>

#ifndef PELIB_AMPLOUTPUTDATA
#define PELIB_AMPLOUTPUTDATA

namespace pelib
{
	class AmplOutputData: public AmplDataOutput, public AmplDataParser
	{
		public:
			AmplOutputData(bool strict) : AmplDataParser(strict) {}
	  virtual ~AmplOutputData() {}
			virtual
			AmplOutputData*
			clone() const = 0;
	  
			
			virtual
			AmplOutputData*
			clone() const = 0;
		protected:
		private:		
	};
}

#endif
