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

#include <pelib/AlgebraData.hpp>

#ifndef PELIB_VECTOR
#define PELIB_VECTOR

namespace pelib
{
	template <class Key, class Value> 
	class Vector: public AlgebraData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			Vector(const std::string name, const VectorType values) : AlgebraData(name), values(values)
			{
				// Do nothing
			}

			Vector(const Vector<Key, Value>* vector): AlgebraData(vector->getName()), values(vector->getValues())
			{
				// Do nothing
			}

			virtual
			Vector*
			clone() const
			{
				return new Vector<Key, Value>(name, values);
			}

			virtual
			const std::map<Key, Value>&
			getValues() const
			{
				return values;
			}

			virtual
			const Value&
			find(Key key) const
			{
				return values.find(key)->second;
			}

			virtual
			size_t
			getSize() const
			{
				return values.size();
			}
			
		protected:
			VectorType values;
		private:		
	};
}

#endif
