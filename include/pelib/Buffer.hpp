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

#include <string>

#ifndef PELIB_BUFFER
#define PELIB_BUFFER

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class Buffer
	{
		public:
			enum class MemoryType: int { undefined = 0, exclusive = 1, shared = 2, distributed = 3, smallAndCheap = 4, largeAndCostly = 8 };
			static const std::string undefined, exclusive, shared, distributed, smallAndCheap, largeAndCostly;
			static const unsigned int memoryAccessMask;
			static const unsigned int memoryCostMask;
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			Buffer(size_t size, const std::string &type, MemoryType memory, unsigned int level, const unsigned int core);

			size_t
			getSize() const;

			unsigned int
			getCore() const;

			MemoryType
			getMemoryType() const;

			std::string
			getMemoryTypeAsString() const;

			std::string
			getHeader() const;
		
			std::string
			getType() const;

			unsigned int
			getLevel() const;

			static std::string
			memoryTypeToString(MemoryType memory);

			static MemoryType
			stringToMemoryType(const std::string &str);

			static Buffer
			nullBuffer();
		protected:
			/** Producer and consumer task pointers **/
			std::string type;
			size_t size;
			std::string header;
			MemoryType memory;
			unsigned int core, level;

		private:		
	};
}

#endif
