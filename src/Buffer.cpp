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
#include <iostream>
#include <sstream>
#include <string.h>

#include <pelib/Buffer.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Buffer::Buffer(size_t size, const string &type, MemoryType memory, unsigned int level, const unsigned int core)
	{
		this->type = string(type);
		this->size = size;
		this->core = core;
		this->memory = memory;
		this->header = header;
		this->level = level;
	}

	unsigned int
	Buffer::getCore() const
	{
		return this->core;
	}

	size_t
	Buffer::getSize() const
	{
		return this->size;
	}

	Buffer::MemoryType
	Buffer::getMemoryType() const
	{
		return this->memory;
	}

	std::string
	Buffer::getMemoryTypeAsString() const
	{
		return Buffer::memoryTypeToString(this->getMemoryType());
	}

	std::string
	Buffer::getHeader() const
	{
		return this->header;
	}

	unsigned int
	Buffer::getLevel() const
	{
		return this->level;
	}

	std::string
	Buffer::getType() const
	{
		return this->type;
	}

	const string Buffer::undefined = "DRAKE_MEMORY_UNDEFINED";
	const string Buffer::exclusive = "DRAKE_MEMORY_PRIVATE";
	const string Buffer::shared = "DRAKE_MEMORY_SHARED";
	const string Buffer::distributed = "DRAKE_MEMORY_DISTRIBUTED";
	const string Buffer::smallAndCheap = "DRAKE_MEMORY_SMALL_CHEAP";
	const string Buffer::largeAndCostly = "DRAKE_MEMORY_LARGE_COSTLY";
	const unsigned int Buffer::memoryAccessMask = 3;
	const unsigned int Buffer::memoryCostMask = 12;
	string
	Buffer::memoryTypeToString(MemoryType memory)
	{
		if(memory == MemoryType::undefined)
		{
			return Buffer::undefined;
		}

		MemoryType access = (MemoryType)((int)memory & 3); // Keep only 2 weakest bits
		MemoryType type = (MemoryType)((int)memory & 12); // Keep the 2 next weakest bits

		string str;
		switch(access)
		{
			case Buffer::MemoryType::exclusive:
				str = Buffer::exclusive;
			break;
			case Buffer::MemoryType::shared:
				str = Buffer::shared;
			break;
			case Buffer::MemoryType::distributed:
				str = Buffer::distributed;
			break;
			default:
				stringstream ss;
				ss << (int)access;
				throw PelibException(string("Unknown memory access type: ") + ss.str());
			break;
			break;
		}

		switch(type)
		{
			case Buffer::MemoryType::smallAndCheap:
				str += string(" | ") + Buffer::smallAndCheap;
			break;
			case Buffer::MemoryType::largeAndCostly:
				str += string(" | ") + Buffer::largeAndCostly;
			break;
			default:
				stringstream ss;
				ss << (int)type;
				throw PelibException(string("Unknown memory cost type: ") + ss.str());
			break;
		}

		return str;
	}

	Buffer
	Buffer::nullBuffer()
	{
		return Buffer(0, "", MemoryType::undefined, 0, 0); 
	}

	Buffer::MemoryType
	Buffer::stringToMemoryType(const std::string &mem)
	{
		char *str = (char*)malloc(sizeof(char) * mem.length() + 1);
		strcpy(str, mem.c_str());
  		char *pch;
  		pch = strtok (str, "| \n\t\r");
		int memory = 0;
		while(pch != NULL)
		{
			string token = string(pch);

			if(token.compare(Buffer::undefined) == 0)
			{
				// This is a neutral element, do nothing
				memory = memory | (int)Buffer::MemoryType::undefined;
			}
			else if(token.compare(Buffer::exclusive) == 0)
			{
				memory = memory | (int)Buffer::MemoryType::exclusive;
			}
			else if(token.compare(Buffer::shared) == 0)
			{
				memory = memory | (int)Buffer::MemoryType::distributed;
			}
			else if(token.compare(Buffer::distributed) == 0)
			{
				memory = memory | (int)Buffer::MemoryType::distributed;
			}
			else if(token.compare(Buffer::smallAndCheap) == 0)
			{
				memory = memory | (int)Buffer::MemoryType::smallAndCheap;
			}
			else if(token.compare(Buffer::largeAndCostly) == 0)
			{
				memory = memory | (int)Buffer::MemoryType::largeAndCostly;
			}
			else
			{
				throw PelibException(string("Invalid memory feature: \"") + token + string("\""));
			}

    			pch = strtok (NULL, "| \n\r\t");
		}
	
		free(str);
		return (Buffer::MemoryType)memory;
	}
}
