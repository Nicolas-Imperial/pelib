#include <string>

#include <AmplDataOutput.hpp>

#ifndef PELIB_AMPLOUTPUTDATAOUTPUT
#define PELIB_AMPLOUTPUTDATAOUTPUT

namespace pelib
{
	class AmplOutputDataOutput: public AmplDataOutput
	{
		public:
			virtual AmplOutputDataOutput* clone() const = 0;
			
		protected:
		private:
	};
}

#endif
