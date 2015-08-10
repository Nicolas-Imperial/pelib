#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/CastException.hpp>

namespace pelib
{
	CastException::CastException(const std::string &message): msg(message)
	{
		// Do nothing else
	}

	CastException::CastException(const char *message): msg(message)
	{
		// Do nothing else
	}

	CastException::~CastException() throw()
	{
		// Do nothing
	}

	const char*
	CastException::what() const throw()
	{
		return std::string("Casting exception: ").append(this->msg).c_str();
	}
}
