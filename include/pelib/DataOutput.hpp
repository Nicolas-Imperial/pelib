/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/Data.hpp>

#ifndef PELIB_DATAOUTPUT
#define PELIB_DATAOUTPUT

namespace pelib
{
	/** Ancestor class of all pelib::Data output classes **/
	class DataOutput
	{
		public:	
			/** Returns a pointer to a copy of this instance **/
			virtual DataOutput* clone() const = 0;
			/** Destructor **/
			virtual ~DataOutput();
			
		protected:
		private:		
	};
}

#endif
