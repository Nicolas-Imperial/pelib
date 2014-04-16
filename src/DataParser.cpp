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
		//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
			return match;
		}
		else
		{
			//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
			throw ParseException(std::string("String \"").append(str).append("\" doesn't match regex \"").append(regex).append("\". "));
		}
	}
}