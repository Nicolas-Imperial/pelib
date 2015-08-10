#include <string>
#include <cstdlib>

#ifndef PELIB_PARSEEXCEPTION
#define PELIB_PARSEEXCEPTION

namespace pelib
{
	class ParseException: public std::exception // or std::runtime_error?
	{
		public:
			explicit ParseException(const std::string &msg);
			explicit ParseException(const char *msg);

			virtual
			~ParseException() throw();

			virtual const char*
			what() const throw();

		protected:
			std::string msg;
	};
}

#endif
