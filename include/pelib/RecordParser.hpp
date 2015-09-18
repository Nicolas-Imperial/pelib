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


#include <sstream>

#ifndef PELIB_RECORDPARSER
#define PELIB_RECORDPARSER

namespace pelib
{
	/** Ancestor class of all parsers of data collections **/
	class RecordParser
	{
		public:
			/** Returns an empty input stream **/
			std::istream& getEmptyStream();
			/** Virtual destructor **/
			virtual ~RecordParser();
		protected:
			/** Empty stream returned by the getEmptyStream method **/
			std::istringstream emptyStream;
		private:
	};
}

#endif
