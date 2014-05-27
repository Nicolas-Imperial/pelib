#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AlgebraData.hpp>
#include <DataOutput.hpp>

#ifndef PELIB_ALGEBRADATAOUTPUT
#define PELIB_ALGEBRADATAOUTPUT

namespace pelib
{
	class AlgebraDataOutput : public DataOutput
	{
		public:	
			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const = 0;

		protected:
		private:		
	};
}

#endif
