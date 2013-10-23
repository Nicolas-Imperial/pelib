#include <iostream>

#include <Data.hpp>

namespace pelib
{
	Data::~Data(){}
	
	Data::Data(std::string name)
	{
		this->name = name;
	}

	Data::Data(Data* data) : name(data->getName())
	{
		// Do nothing else
	}
	
	void
	Data::setName(const std::string name)
	{
		this->name = name;
	}
	
	const std::string&
	Data::getName() const
	{
		return this->name;
	}
}