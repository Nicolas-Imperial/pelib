#include "Schedule.hpp"

#include <ScheduleParser.hpp>

#ifndef PELIB_XMLSCHEDULEPARSER_H
#define PELIB_XMLSCHEDULEPARSER_H

namespace pelib
{
	class XMLScheduleParser : public ScheduleParser
	{
		public:
			virtual Schedule* parse(std::istream &is) = 0;
	};
}

#endif
