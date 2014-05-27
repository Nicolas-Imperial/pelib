#include <iostream>

#include <AlgebraData.hpp>

namespace pelib
{
	AlgebraData::AlgebraData(const std::string name)
	{
		this->name = name;
	}

	AlgebraData::AlgebraData(const AlgebraData& data)
	{
		this->name = data.getName();
	}
	
	void
	AlgebraData::setName(const std::string name)
	{
		this->name = name;
	}
	
	const std::string&
	AlgebraData::getName() const
	{
		return this->name;
	}
}
