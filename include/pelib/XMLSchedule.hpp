#include <pelib/Schedule.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/XMLScheduleParser.hpp>
#include <pelib/XMLScheduleOutput.hpp>

#ifndef PELIB_XMLSCHEDULE_H
#define PELIB_XMLSCHEDULE_H

namespace pelib
{
	class XMLSchedule : public XMLScheduleParser, public XMLScheduleOutput
	{
		public:
			virtual ~XMLSchedule();
			virtual	void dump(std::ostream &os, const Schedule *data, const Taskgraph *tg, const Platform *pt) const;
			virtual	void dump(std::ostream &os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const;
			virtual Schedule* parse(std::istream &is) const;
			virtual XMLSchedule* clone() const;
	};
}

#endif