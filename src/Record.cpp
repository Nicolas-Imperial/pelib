#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Record.hpp>
#include <AmplDataParser.hpp>
#include <Data.hpp>
#include <DataOutput.hpp>

namespace pelib
{	
	Record::~Record()
	{
		deleteRecords();
	}

	std::map<std::string, Data*>
	Record::getAllRecords() const
	{
		return this->records;
	}

	void
	Record::insert(const pelib::Data *data)
	{
		records.insert(std::pair<std::string, pelib::Data*>(data->getName(), data->clone()));
	}

	Record&
	Record::operator=(const Record &rhs)
	{
		// Free all records
		deleteRecords();
		
		// Clone every objects of the source's collection
		std::map<std::string, Data*> rhrec = rhs.records; 
		for (std::map<std::string, Data*>::iterator i = rhrec.begin(); i != rhrec.end(); i++)
		{
			Data *copy = i->second->clone();
			this->records.insert(std::pair<std::string, Data*>(i->first, copy));
		}
		
		return *this;
	}

	void
	Record::deleteRecords()
	{
		for (std::map<std::string, Data*>::iterator i = records.begin(); i != records.end();)
		{
			delete i->second;
			std::map<std::string, Data*>::iterator erase = i;
			i++;
			records.erase(erase);
		}
	}
}