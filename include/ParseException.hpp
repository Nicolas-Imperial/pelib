#include <string>
#include <cstdlib>

#ifndef PELIB_PARSEEXCEPTION
#define PELIB_PARSEEXCEPTION

namespace pelib
{
	class ParseException: std::exception
	{
		public:
			ParseException(std::string message) throw();

			virtual
			~ParseException() throw();

			const char*
			what();

		protected:
			std::string message;
	};
}

#endif
