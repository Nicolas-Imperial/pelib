#include <Platform.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_PLATFORMOUTPUT
#define PELIB_PLATFORMOUTPUT

namespace pelib
{
	class PlatformOutput : public StreamingAppDataOutput
	{
		public:	
			virtual	void dump(std::ostream& o, const StreamingAppData *data) const = 0;
			virtual PlatformOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
