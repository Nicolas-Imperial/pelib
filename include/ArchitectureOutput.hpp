#include <Architecture.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_ARCHITECTUREOUTPUT
#define PELIB_ARCHITECTUREOUTPUT

namespace pelib
{
	class ArchitectureOutput : public StreamingAppDataOutput
	{
		public:	
			virtual	void dump(std::ostream& o, const StreamingAppData *data) const = 0;
			virtual ArchitectureOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
