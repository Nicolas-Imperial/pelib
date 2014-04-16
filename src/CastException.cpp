#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <CastException.hpp>

namespace pelib
{
	CastException::CastException(std::string message) throw()
	{
		this->message = message;
	}

	CastException::~CastException() throw() {}

	const char*
	CastException::what()
	{
		return std::string("Casting exception: ").append(this->message).c_str();
	}
}
