#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplRecordParser.hpp>

namespace pelib
{
	boost::cmatch
	AmplRecordParser::match(std::string regex, std::string str)
	{
		boost::cmatch match;
		boost::regex param_scalar(regex);

		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			return match;
		}
		else
		{
			throw ParseException("Text doesn't match regex");
		}
	}
}
