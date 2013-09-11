#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>	

#ifndef PARSEEXCEPTION
#define PARSEEXCEPTION

namespace pelib
{
	class ParseException: std::exception
	{
		protected:
		std::string message;

		public:
		ParseException(std::string message) throw();

		virtual
		~ParseException() throw();

		const char*
		what();
	};
}

#endif
