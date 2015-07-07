#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/CastException.hpp>
#include <pelib/CppData.hpp>
#include <pelib/Data.hpp>

namespace pelib
{
	const std::string
	CppData::type_name(const std::type_info &type) const
	{
		int int_var;
		float float_var;
		double double_var;

		if(type == typeid(int_var))
		{
			return std::string("int").c_str();
		}
		else if(type == typeid(float_var))
		{
			return std::string("float").c_str();
		}
		else if(type == typeid(double_var))
		{
			return std::string("double").c_str();
		}
		else
		{
			throw CastException("Type is not int, float or double");
		}
	}
}
