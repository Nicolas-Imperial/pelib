#include <string>

#include <pelib/AmplDataOutput.hpp>

#ifndef PELIB_AMPLINPUTDATAOUTPUT
#define PELIB_AMPLINPUTDATAOUTPUT

namespace pelib
{
	class AmplInputDataOutput: public AmplDataOutput
	{
		public:
			virtual AmplInputDataOutput* clone() const = 0;

		protected:
		private:
	};
}

#endif
