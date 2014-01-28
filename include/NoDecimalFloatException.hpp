#include <cstdlib>

#ifndef PELIB_NODECIMALFLOATEXCEPTION
#define PELIB_NODECIMALFLOATEXCEPTION

namespace pelib
{
	class NoDecimalFloatException: std::exception
	{
		public:
			NoDecimalFloatException(std::string message, float value) throw();

			virtual
			~NoDecimalFloatException() throw();

			const char*
			what();

			const float
			getValue();

		protected:
			std::string message;
			float value;
	};
}

#endif
