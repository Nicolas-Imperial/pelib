#include <AmplInputDataParser.hpp>
#include <AmplInputDataOutput.hpp>

#ifndef PELIB_AMPLINPUTDATA
#define PELIB_AMPLINPUTDATA

namespace pelib
{
	class AmplInputData: public AmplInputDataOutput, public AmplInputDataParser
	{
		public:
/*
			AmplInputData(bool strict) : AmplInputDataParser(strict) {}
			virtual ~AmplInputData(){}
			virtual	AmplInputData* clone() const = 0;
*/
		protected:
		private:		
	};
}

#endif
