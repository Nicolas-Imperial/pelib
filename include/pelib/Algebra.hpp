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
#include <map>

#include <pelib/AlgebraData.hpp>
#include <pelib/Record.hpp>
#include <pelib/constraints.hpp>

#ifndef PELIB_ALGEBRA
#define PELIB_ALGEBRA
namespace pelib
{
	class Algebra : public Record
	{
		public:
			Algebra();
			Algebra(const std::map<std::string, const AlgebraData * const>&);
			Algebra(const Algebra &rhs);
			virtual	~Algebra();

			const std::map<std::string, const AlgebraData * const>&
			getAllRecords() const;

			std::map<std::string, const int *>&
			copyRecords();

			//void merge(const Record& rec) {};
			virtual Algebra
			merge(const Algebra&) const; 

			template<class T>
			std::map<std::string, const T* const>
			getRecords() const
			{
				// Make sure T is derived from AlgebraData
				Derived_from<T, AlgebraData>();

				// Build a new map containing all elements from records that could be casted to Algebra
				std::map<std::string, const T * const> record;

				for (std::map<std::string, const AlgebraData * const>::const_iterator i = records.begin(); i != records.end(); i++)
				{
					// Try to cast this type


					const AlgebraData *ptr = i->second;

					T* elem = dynamic_cast<T*>(ptr);
					
					if(elem != NULL)
					{
						record.insert(std::pair<std::string, const T * const>(i->first, elem));
					}
				}

				return record;
			}

			template<class T>
			const T * const
			find(std::string key) const
			{
				// Make sure T is derived from AlgebraData
				Derived_from<T, AlgebraData>();

				typename std::map<std::string, const AlgebraData * const>::const_iterator iter = records.find(key);
				if(iter != records.end())
				{
					const T * const elem = dynamic_cast<const T * const >(iter->second);

					if(elem != NULL)
					{
						return elem;
					}
					else
					{
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}

			void insert(const Data *data) {}
			virtual void insert(const pelib::AlgebraData *data);

			virtual void
			remove(const std::string name);

			virtual
			Algebra&
			operator=(const Algebra &rhs);
			
		protected:
			std::map<std::string, const AlgebraData * const> records;

			void
			deleteRecords();	
		private:
	};
}

#endif
