#include <Architecture.hpp>
#include <ArchitectureOutput.hpp>

#ifndef PELIB_AMPLARCHITECTUREOUTPUT
#define PELIB_AMPLARCHITECTUREOUTPUT

namespace pelib
{
	class AmplArchitectureOutput : public ArchitectureOutput
	{
		public:
			virtual	void dump(std::ostream &os, const Architecture *sched) const = 0;
	};
}

#endif
