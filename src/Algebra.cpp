#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Algebra.hpp>
#include <AmplDataParser.hpp>
#include <AlgebraData.hpp>
#include <TaskgraphRecord.hpp>

namespace pelib
{	
	Algebra::Algebra() { /* Do nothing */ }

	Algebra::~Algebra()
	{
		deleteRecords();
	}

	Algebra::Algebra(const Algebra &rhs)
	{
		*this = rhs;
	}

	void
	Algebra::merge(const Algebra& record)
	{
		// Call what a record would do
		merge(record);

		// Copy all AlgebraData
		for(auto iter = record.getAllRecords().begin(); iter != record.getAllRecords().end(); iter++)
		{
			records.insert(std::pair<std::string, pelib::AlgebraData*>(iter->second->getName(), iter->second->clone()));
		}
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
