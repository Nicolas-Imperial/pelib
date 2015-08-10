#include <string>
#include <cstdlib>

#ifndef PELIB_CASTEXCEPTION
#define PELIB_CASTEXCEPTION

namespace pelib
{
	class CastException: public std::exception
	{
		public:
			explicit CastException(const std::string &message);
			explicit CastException(const char *message);

			virtual
			~CastException() throw();

			virtual const char*
			what() const throw();

		protected:
			std::string msg;
	};
}

#endif
