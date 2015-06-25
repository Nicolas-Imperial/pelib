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
			virtual Schedule* parse(std::istream &is, Taskgraph &tg) const;
			virtual	void dump(std::ostream &os, const Record *data) const;
			virtual	void dump(std::ostream &os, const Record &data) const;
			virtual XMLSchedule* clone() const;
	};
}

#endif
