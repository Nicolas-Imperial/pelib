#include "Schedule.hpp"

#include <ScheduleOutput.hpp>

#ifndef PELIB_XMLSCHEDULEOUTPUT_H
#define PELIB_XMLSCHEDULEOUTPUT_H

namespace pelib
{
	class XMLScheduleOutput : public ScheduleOutput
	{
		public:
			virtual	void dump(std::ostream &os, const Schedule *sched) const = 0;
	};
}

#endif
