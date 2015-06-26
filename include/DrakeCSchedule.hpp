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
			virtual	void dump(std::ostream &os, const Record *data) const;
			virtual	void dump(std::ostream &os, const Record &data) const;
			virtual DrakeCSchedule* clone() const;
	};
}

#endif
