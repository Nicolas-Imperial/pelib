#include <AmplDataParser.hpp>
#include <AmplDataOutput.hpp>

#ifndef PELIB_AMPLOUTPUTDATA
#define PELIB_AMPLOUTPUTDATA

namespace pelib
{
	class AmplOutputData: public AmplDataOutput, public AmplDataParser
	{
		public:
			virtual
			AmplOutputData*
			clone() const = 0;
		protected:
		private:		
	};
}

#endif
