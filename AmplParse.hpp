#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

using namespace std;
using namespace boost;

namespace pelib
{

class ParseException: std::exception
{
	protected:
	string message;

	public:
	ParseException(string message) throw()
	{
		this->message = string(": ").append(message);
	}

	virtual
	~ParseException() throw() {}

	const char*
	what()
	{
		return string("Parsing exception").append(this->message).c_str();
	}
};

}
