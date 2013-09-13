#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <ParseException.hpp>

namespace pelib
{
	ParseException::ParseException(std::string message) throw()
	{
		this->message = std::string(": ").append(message);
	}

	ParseException::~ParseException() throw() {}

	const char*
	ParseException::what()
	{
		return std::string("Parsing exception").append(this->message).c_str();
	}
}
