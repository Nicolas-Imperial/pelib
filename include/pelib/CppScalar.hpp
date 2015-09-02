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


#include <pelib/CppData.hpp>
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_CPPSCALAR
#define PELIB_CPPSCALAR

namespace pelib
{
	template <class Value>
	class
	CppScalar: public CppData
	{
		public:
			CppScalar() : CppData()
			{
				// Do nothing
			}
			
			virtual
			CppScalar*
			clone() const
			{
				return new CppScalar();
			}

			virtual
			void
			dump(std::ostream &stream, const AlgebraData *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				stream << "pelib::Scalar<" << type_name(typeid(scalar->getValue())).c_str() << "> " << scalar->getName() << "(\"" << scalar->getName() << "\", " << scalar->getValue() << ");" << std::endl;
			}
	
		protected:
		private:		
	};
}

#endif
