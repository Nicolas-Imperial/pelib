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
#include <set>

#include <pelib/AlgebraData.hpp>

#ifndef PELIB_SET
#define PELIB_SET

namespace pelib
{
	template <class Value> 
	class Set: public AlgebraData
	{
		typedef std::set<Value> SetType;
		
		public:
			Set(const std::string name, const SetType values) : AlgebraData(name), values(values)
			{
				// Do nothing
			}

			Set(const Set<Value>* Set): AlgebraData(Set->getName()), values(Set->getValues())
			{
				// Do nothing
			}

			virtual
			Set*
			clone() const
			{
				return new Set<Value>(name, values);
			}

			virtual
			const SetType&
			getValues() const
			{
				return values;
			}

			virtual
			bool
			find(Value elem) const
			{
				return getValues().find(elem) != getValues().end();
			}

			virtual
			size_t
			getSize() const
			{
				return values.size();
			}
			
		protected:
			SetType values;
		private:		
	};
}

#endif
