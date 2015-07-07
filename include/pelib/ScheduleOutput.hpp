#include <pelib/Schedule.hpp>
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_SCHEDULEOUTPUT
#define PELIB_SCHEDULEOUTPUT

namespace pelib
{
	class ScheduleOutput : public RecordOutput
	{
		public:	
			virtual ScheduleOutput* clone() const = 0;
			virtual	void dump(std::ostream &os, const Schedule *data, const Taskgraph *tg, const Platform *pt) const = 0;
			virtual	void dump(std::ostream &os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const = 0;

		protected:
		private:		
	};
}

#endif
