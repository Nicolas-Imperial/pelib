#include <cstdlib>

#ifndef PELIB_CASTEXCEPTION
#define PELIB_CASTEXCEPTION

namespace pelib
{
	class CastException: std::exception
	{
		public:
			CastException(std::string message) throw();

			virtual
			~CastException() throw();

			const char*
			what();

		protected:
			std::string message;
	};
}

#endif
