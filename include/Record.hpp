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
	class Record
	{
		public:
			virtual
			~Record();
			
			std::map<std::string, Data*>
			getAllRecords() const;

			template<class T>
			std::map<std::string, T*>
			getRecords() const
			{
				// Build a new map containing all elements from records that could be casted to Record
				std::map<std::string, T*> record;

				for (std::map<std::string, Data*>::const_iterator i = records.begin(); i != records.end(); i++)
				{
					try
					{
						T* elem = dynamic_cast<T*>(i->second->clone());
						record.insert(std::pair<std::string, T*>(i->first, elem));
					} catch(std::exception &e)
					{
						// Couldn't cast the element to record: just let that go and try again with next element
					}
				}

				return record;
			}

			template<class T>
			T*
			find(std::string key) const
			{
				std::map<std::string, T*> rec = this->getRecords<T>();
				return rec.find(key)->second;
			}

			void
			insert(pelib::Data *data);

			virtual
			Record&
			operator=(const Record &rhs);
			
		protected:
			std::map<std::string, Data*> records;

			void
			deleteRecords();
			
		private:
	};
}

#endif
