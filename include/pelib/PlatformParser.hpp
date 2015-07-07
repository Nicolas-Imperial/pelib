#include <pelib/Platform.hpp>
#include <pelib/DataParser.hpp>

#ifndef PELIB_PLATFORMPARSER
#define PELIB_PLATFORMPARSER

namespace pelib
{
	class PlatformParser : public DataParser
	{
		public:
			virtual
			Platform*
			parse(std::istream &in) const = 0;

			virtual PlatformParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
