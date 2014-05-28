#include <Architecture.hpp>

#include <AmplArchitectureParser.hpp>
#include <AmplArchitectureOutput.hpp>

#ifndef PELIB_AMPLARCHITECTURE
#define PELIB_AMPLARCHITECTURE

namespace pelib
{
	class AmplArchitecture : public AmplArchitectureParser, public AmplArchitectureOutput
	{
		public:
			virtual Architecture* parse(std::istream &is);
			virtual	void dump(std::ostream &os, const StreamingAppData *data) const;
			virtual AmplArchitecture* clone() const;
	};
}

#endif
