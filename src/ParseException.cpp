#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/ParseException.hpp>

namespace pelib
{
	ParseException::ParseException(const std::string &message): msg(message) 
	{
		// Do nothing else
	}

	ParseException::ParseException(const char *message): msg(message) 
	{
		// Do nothing else
	}

	ParseException::~ParseException() throw() {}

	const char*
	ParseException::what() const throw()
	{
		return std::string("Parsing exception: ").append(this->msg).c_str();
	}
}
