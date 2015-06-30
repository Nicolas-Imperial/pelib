#include "Schedule.hpp"

#include <DrakeCScheduleOutput.hpp>

#ifndef PELIB_DRAKECSCHEDULE_H
#define PELIB_DRAKECSCHEDULE_H

namespace pelib
{
	class DrakeCSchedule : public DrakeCScheduleOutput
	{
		public:
			virtual ~DrakeCSchedule();
			virtual	void dump(std::ostream &os, const Schedule *data, const Taskgraph *tg, const Platform *pt) const;
			virtual	void dump(std::ostream &os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const;
			virtual DrakeCSchedule* clone() const;
	};
}

#endif
