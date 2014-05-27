#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_DATAOUTPUT
#define PELIB_DATAOUTPUT

namespace pelib
{
	class DataOutput
	{
		public:	
			virtual DataOutput* clone() const = 0;
			
		protected:
		private:		
	};
}

#endif
