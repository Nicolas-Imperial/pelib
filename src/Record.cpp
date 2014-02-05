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

#include <Scalar.hpp>

namespace pelib
{	
	Record::~Record()
	{
		deleteRecords();
	}

	const std::map<std::string, const Data * const>&
	Record::getAllRecords() const
	{
		return records;
	}

	void
	Record::insert(const pelib::Data *data)
	{
		std::map<std::string, const Data * const>::iterator iter = records.find(data->getName());
		if(iter != records.end())
		{
			const Data *element = iter->second;
			records.erase(iter);
			delete element;
		}
		records.insert(std::pair<std::string, pelib::Data*>(data->getName(), data->clone()));
	}

	void
	Record::remove(const std::string name)
	{
		std::map<std::string, const Data * const>::iterator ptr = records.find(name);

		if(ptr != records.end())
		{
			delete ptr->second;
			records.erase(name);
		}
	}

	Record&
	Record::operator=(const Record &rhs)
	{
		// Free all records
		deleteRecords();
		
		// Clone every objects of the source's collection
		std::map<std::string, const Data * const> rhrec = rhs.records; 
		for (std::map<std::string, const Data * const>::iterator i = rhrec.begin(); i != rhrec.end(); i++)
		{
			Data *copy = i->second->clone();
			this->records.insert(std::pair<std::string, const Data * const>(i->first, copy));
		}
		
		return *this;
	}

	void
	Record::deleteRecords()
	{
		for (std::map<std::string, const Data * const>::iterator i = records.begin(); i != records.end();)
		{
			delete i->second;
			std::map<std::string, const Data * const>::iterator erase = i;
			i++;
			records.erase(erase);
		}
	}
}
