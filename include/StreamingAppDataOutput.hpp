#include <iostream>

#include <StreamingAppData.hpp>
#include <DataOutput.hpp>

#ifndef PELIB_STREAMINGAPPDATAOUTPUT
#define PELIB_STREAMINGAPPDATAOUTPUT

namespace pelib
{
	class StreamingAppDataOutput : public DataOutput
	{
		public:	
			virtual
			StreamingAppDataOutput*
			clone() const = 0;
		
			virtual void dump(std::ostream &str, const StreamingAppData *data) const = 0;
			virtual void dump(std::ostream &str, const StreamingAppData &data) const = 0;

		protected:
		private:		
	};
}

#endif
