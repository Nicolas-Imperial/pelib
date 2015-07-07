#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/Algebra.hpp>
#include <pelib/AmplDataParser.hpp>
#include <pelib/AlgebraData.hpp>

namespace pelib
{	
	Algebra::Algebra() { /* Do nothing */ }
	
	Algebra::Algebra(const std::map<std::string, const AlgebraData* const> &records)
	{
		this->records = records;
	}

	Algebra::~Algebra()
	{
		deleteRecords();
	}

	Algebra::Algebra(const Algebra &rhs)
	{
		*this = rhs;
	}

	Algebra
	Algebra::merge(const Algebra& record) const
	{
		// Copy all AlgebraData
		std::map<std::string, const AlgebraData * const> rec;

		// Copy all records stored in this instance
		for(std::map<std::string, const AlgebraData * const>::const_iterator iter = records.begin(); iter != records.end(); iter++)
		{
			rec.insert(std::pair<std::string, pelib::AlgebraData*>(iter->second->getName(), iter->second->clone()));
		}

		// Copy all records from the foreign instance
		for(std::map<std::string, const AlgebraData * const>::const_iterator iter = record.getAllRecords().begin(); iter != record.getAllRecords().end(); iter++)
		{
			rec.insert(std::pair<std::string, pelib::AlgebraData*>(iter->second->getName(), iter->second->clone()));
		}

		// Build a new algebra with all records
		return Algebra(rec);
	}

	const std::map<std::string, const AlgebraData * const>&
	Algebra::getAllRecords() const
	{
		return records;
	}

	void
	Algebra::insert(const pelib::AlgebraData *data)
	{
		std::map<std::string, const AlgebraData * const>::iterator iter = records.find(data->getName());
		if(iter != records.end())
		{
			const AlgebraData *element = iter->second;
			records.erase(iter);
			delete element;
		}
		records.insert(std::pair<std::string, pelib::AlgebraData*>(data->getName(), data->clone()));
	}

	void
	Algebra::remove(const std::string name)
	{
		std::map<std::string, const AlgebraData * const>::iterator ptr = records.find(name);

		if(ptr != records.end())
		{
			delete ptr->second;
			records.erase(name);
		}
	}

	Algebra&
	Algebra::operator=(const Algebra &rhs)
	{
		if(this != &rhs)
		{
			// Free all records
			deleteRecords();
			
			// Clone every objects of the source's collection
			std::map<std::string, const AlgebraData * const> rhrec = rhs.records; 
			for (std::map<std::string, const AlgebraData * const>::iterator i = rhrec.begin(); i != rhrec.end(); i++)
			{
				AlgebraData *copy = i->second->clone();
				this->records.insert(std::pair<std::string, const AlgebraData * const>(i->first, copy));
			}
		}
		
		return *this;
	}

	void
	Algebra::deleteRecords()
	{
		for (std::map<std::string, const AlgebraData * const>::iterator i = records.begin(); i != records.end();)
		{
			delete i->second;
			std::map<std::string, const AlgebraData * const>::iterator erase = i;
			i++;
			records.erase(erase);
		}
	}
}
