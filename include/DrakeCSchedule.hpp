#include "Schedule.hpp"

#include <DrakeCScheduleOutput.hpp>

#ifndef PELIB_DRAKECSCHEDULE_H
#define PELIB_DRAKECSCHEDULE_H

namespace pelib
{
	class DrakeCSchedule : public DrakeCScheduleOutput
	{
		public:
			virtual	void dump(std::ostream &os, const Data *data) const;
			virtual	void dump(std::ostream &os, const Data &data) const;
			virtual DrakeCSchedule* clone() const;
	};
}

#endif
