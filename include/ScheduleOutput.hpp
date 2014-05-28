#include <Schedule.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_SCHEDULEOUTPUT
#define PELIB_SCHEDULEOUTPUT

namespace pelib
{
	class ScheduleOutput : public StreamingAppDataOutput
	{
		public:	
			virtual ScheduleOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
