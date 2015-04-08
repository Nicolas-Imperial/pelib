#include "Schedule.hpp"

#include <SnekkjaCScheduleOutput.hpp>

#ifndef PELIB_SNEKKJACSCHEDULE_H
#define PELIB_SNEKKJACSCHEDULE_H

namespace pelib
{
	class SnekkjaCSchedule : public SnekkjaCScheduleOutput
	{
		public:
			virtual	void dump(std::ostream &os, const StreamingAppData *data) const;
			virtual	void dump(std::ostream &os, const StreamingAppData &data) const;
			virtual SnekkjaCSchedule* clone() const;
	};
}

#endif
