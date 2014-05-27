#include <StreamingAppParser.hpp>
#include <StreamingAppOutput.hpp>

#include <ScheduleParser.hpp>
#include <ScheduleOutput.hpp>

#ifndef PELIB_STREAMINGAPPHANDLER
#define PELIB_STREAMINGAPPHANDLER

namespace pelib
{
	class StreamingAppHandler: public StreamingAppParser, public StreamingAppOutput
	{
		public:
			StreamingAppHandler(std::vector<ScheduleParser*> scheduleParsers,
			std::vector<ScheduleOutput*> scheduleOutputs);
			StreamingAppHandler();

			virtual
			~StreamingAppHandler();

			StreamingApp
			parse(std::istream &data);

			virtual
			void
			dump(std::ostream& o, const StreamingApp &record) const;

			virtual
			void
			dumpSchedule(std::ostream& o, const Schedule *data) const;

			virtual
			StreamingAppHandler&
			operator=(const StreamingAppHandler &rhs);

		protected:
			std::vector<ScheduleParser*> scheduleParsers;
			std::vector<ScheduleOutput*> scheduleOutputs;

			void
			deleteParsers();

			void
			deleteOutputs();

			void
			addParsers();

			void
			addOutputs();
			
		private:
	};
}

#endif

