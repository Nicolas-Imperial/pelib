#include <Architecture.hpp>
#include <StreamingAppDataParser.hpp>

#ifndef PELIB_ARCHITECTUREPARSER
#define PELIB_ARCHITECTUREPARSER

namespace pelib
{
	class ArchitectureParser : public StreamingAppDataParser
	{
		public:
			virtual
			Architecture*
			parse(std::istream &in) = 0;

			virtual ArchitectureParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
