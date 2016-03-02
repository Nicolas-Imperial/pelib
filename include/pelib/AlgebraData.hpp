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

#include <pelib/Data.hpp>

#ifndef PELIB_ALGEBRADATA
#define PELIB_ALGEBRADATA

namespace pelib
{
	/** Base class of all data classes that a Algebra container can manipulate **/
	class AlgebraData : public Data
	{
		public:
			/** Creates a new instance AlgebraData with name given as argument **/
			AlgebraData(const std::string);

			/** Copy constructor **/
			AlgebraData(const AlgebraData&);

			/** Returns the name of this element **/
			virtual	const std::string&
			getName() const;

			/** Sets the name of this element **/
			virtual void
			setName(const std::string);

			/** Returns a copy of this element **/
			virtual AlgebraData* clone() const = 0;

			/** Merge data in this instance and instance given as argument,
			    or replace it if merging is not possible **/
			virtual void merge(const AlgebraData*) = 0;
		protected:
			/** Stores the name of this element **/
			std::string name;
	};
}

#endif
