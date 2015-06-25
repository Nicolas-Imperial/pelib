#include <Schedule.hpp>
#include <RecordOutput.hpp>

#ifndef PELIB_SCHEDULEOUTPUT
#define PELIB_SCHEDULEOUTPUT

namespace pelib
{
	class ScheduleOutput : public RecordOutput
	{
		public:	
			virtual ScheduleOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
