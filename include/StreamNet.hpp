#include <StreamingAppParser.hpp>
#include <StreamingAppOutput.hpp>

#include <TaskgraphParser.hpp>
#include <PlatformParser.hpp>
#include <ScheduleParser.hpp>

#include <ScheduleOutput.hpp>

#ifndef PELIB_STREAMNET
#define PELIB_STREAMNET

namespace pelib
{
	class StreamNet: public StreamingAppParser, public StreamingAppOutput
	{
		public:
			StreamNet();

			virtual
			~StreamNet();

			virtual StreamingApp
			parse(std::istream &taskgraph, std::istream &platform, std::istream &schedule) const;

			virtual
			void
			dump(std::ostream& o, const StreamingAppData *data) const;
			
			virtual
			void
			dump(std::ostream& o, const StreamingAppData &data) const;

			virtual
			StreamNet&
			operator=(const StreamNet &rhs);

		protected:
			std::vector<TaskgraphParser*> taskgraphParsers;
			std::vector<PlatformParser*> platformParsers;
			std::vector<ScheduleParser*> scheduleParsers;
			std::vector<StreamingAppDataOutput*> outputs;

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

