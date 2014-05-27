#include <Architecture.hpp>
#include <ArchitectureParser.hpp>

#ifndef PELIB_AMPLARCHITECTUREPARSER
#define PELIB_AMPLARCHITECTUREPARSER

namespace pelib
{
	class AmplArchitectureParser : public ArchitectureParser
	{
		public:
			virtual Architecture* parse(std::istream &is) = 0;
	};
}

#endif
