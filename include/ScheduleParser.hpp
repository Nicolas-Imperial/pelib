#include <Schedule.hpp>
#include <StreamingAppDataParser.hpp>

#ifndef PELIB_SCHEDULEPARSER
#define PELIB_SCHEDULEPARSER

namespace pelib
{
	class ScheduleParser : public StreamingAppDataParser
	{
		public:
			virtual
			Schedule*
			parse(std::istream &in) const = 0;

			virtual ScheduleParser* clone() const = 0;

		protected:
		private:
	};
}

#endif
