#include <Schedule.hpp>
#include <StreamingAppDataOutput.hpp>

#ifndef PELIB_SCHEDULEOUTPUT
#define PELIB_SCHEDULEOUTPUT

namespace pelib
{
	class ScheduleOutput : public StreamingAppDataOutput
	{
		public:	
			virtual	void dump(std::ostream& o, const Schedule *data) const = 0;
			virtual ScheduleOutput* clone() const = 0;

		protected:
		private:		
	};
}

#endif
