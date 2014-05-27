#include "Schedule.hpp"

#include <XMLScheduleParser.hpp>
#include <XMLScheduleOutput.hpp>

#ifndef PELIB_XMLSCHEDULE_H
#define PELIB_XMLSCHEDULE_H

namespace pelib
{
	class XMLSchedule : public XMLScheduleParser, public XMLScheduleOutput
	{
		public:
			virtual Schedule* parse(std::istream &is);
			virtual	void dump(std::ostream &os, const Schedule *sched) const;
			virtual XMLSchedule* clone() const;
	};
}

#endif
