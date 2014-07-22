#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AlgebraData.hpp>
#include <Record.hpp>
#include <constraints.hpp>

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
