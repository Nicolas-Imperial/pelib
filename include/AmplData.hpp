#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#ifndef AMPLPARSE
#define AMPLPARSE

#include <AmplRecordParser.hpp>

namespace pelib
{
	class AmplData
	{
		protected:
		std::map<std::string, AmplRecordParser*> records;
		std::vector<AmplRecordParser*> default_parsers;

		public:
		AmplData();

		AmplData(std::vector<AmplRecordParser*> parsers);

		virtual
		~AmplData();		

		std::map<std::string, AmplRecordParser*>
		getAllRecords();

		template<class Record>
		std::map<std::string, Record*>
		getRecords()
		{
			// Build a new map containing all elements from records that could be casted to Record
	//		cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << "Hello world" << endl;
			std::map<std::string, Record*> record;

			for (std::map<std::string, AmplRecordParser*>::iterator i = records.begin(); i != records.end(); i++)
			{
				try
				{
					Record* elem = dynamic_cast<Record*>(i->second);
					record.insert(std::pair<std::string, Record*>(i->first, elem));
				} catch(std::exception &e)
				{
					// Couldn't cast the element to record: just let that go and try again with next element
				}
			}

			return record;
		}

		template<class Record>
		Record*
		find(std::string key)
		{
			std::map<std::string, Record* > rec = this->getRecords<Record>();
			return rec.find(key)->second;
		}

		void
		parse(std::istream &ampl_data);

		void
		parse(std::istream &ampl_data, std::vector<AmplRecordParser*> parsers);

		virtual std::ostream&
		dump(std::ostream& o) const;
	};

	std::ostream&
	operator<<(std::ostream& o, const AmplData& r);
}

#endif
