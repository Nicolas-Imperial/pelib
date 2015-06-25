#include <Platform.hpp>
#include <RecordOutput.hpp>

#ifndef PELIB_PLATFORMOUTPUT
#define PELIB_PLATFORMOUTPUT

namespace pelib
{
	class PlatformOutput : public DataOutput
	{
		public:	
			virtual	void dump(std::ostream& o, const Record *data) const = 0;
			virtual PlatformOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
