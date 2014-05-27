#include <iostream>

#include <AlgebraData.hpp>

namespace pelib
{
	AlgebraData::AlgebraData(const std::string new_name): Data(new_name) {}
	AlgebraData::AlgebraData(const AlgebraData &original): Data(original) {}
}
