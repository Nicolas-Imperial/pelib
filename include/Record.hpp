#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_RECORD
#define PELIB_RECORD
namespace pelib
{
  class Record;
}
#include "TaskgraphRecord.hpp"

	class Record
	{
		public:				
			virtual
			~Record();
			Record();
			Record(const Record &rhs);

	  Record(const TaskgraphRecord& tgr);


			const std::map<std::string, const Data * const>&
			getAllRecords() const;

			std::map<std::string, const int *>&
			copyRecords(); 

			template<class T>
			std::map<std::string, const T* const>
			getRecords() const
			{
				// Build a new map containing all elements from records that could be casted to Record
				std::map<std::string, const T * const> record;

				for (std::map<std::string, const Data * const>::const_iterator i = records.begin(); i != records.end(); i++)
				{
					// Try to cast this type


					Data *ptr = i->second;

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
				typename std::map<std::string, const Data * const>::const_iterator iter = records.find(key);
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

			void
			insert(const pelib::Data *data);

			void
			remove(const std::string name);

			virtual
			Record&
			operator=(const Record &rhs);
			
		protected:
			std::map<std::string, const Data * const> records;

			void
			deleteRecords();
			
		private:
	};
}

#endif
