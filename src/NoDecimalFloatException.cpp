#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/NoDecimalFloatException.hpp>

namespace pelib
{
	NoDecimalFloatException::NoDecimalFloatException(std::string message, float value) throw()
	{
		this->message = message;
		this->value = value;
	}

	NoDecimalFloatException::~NoDecimalFloatException() throw() {}

	const char*
	NoDecimalFloatException::what()
	{
		return std::string("Float parsing exception: ").append(this->message).c_str();
	}

	const float
	NoDecimalFloatException::getValue()
	{
		return this->value;
	}
}
