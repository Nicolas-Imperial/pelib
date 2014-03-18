#include <string>

#include <DataOutput.hpp>

#ifndef PELIB_CPPDATAOUTPUT
#define PELIB_CPPDATAOUTPUT

namespace pelib
{
	class CppDataOutput: public DataOutput
	{
		public:
			virtual
			CppDataOutput*
			clone() const = 0;
			
		protected:
		private:
	};
}

#endif
