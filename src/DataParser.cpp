#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <DataParser.hpp>

namespace pelib
{
	DataParser::DataParser(bool strict)
	{
		this->strict = strict;
	}
	
	boost::cmatch
	DataParser::match(std::string regex, std::string str)
	{
		boost::cmatch match;
		boost::regex param_scalar(regex);

		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			return match;
		}
		else
		{
			throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(regex).append("\". "));
		}
	}
}