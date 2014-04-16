#include <string>

#include <DataOutput.hpp>

#ifndef PELIB_AMPLDATAOUTPUT
#define PELIB_AMPLDATAOUTPUT

namespace pelib
{
	class AmplDataOutput: public DataOutput
	{
		public:
			virtual
			AmplDataOutput*
			clone() const = 0;
			
		protected:
		private:
	};
}

#endif
