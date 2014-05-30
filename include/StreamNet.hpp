#include <StreamingAppParser.hpp>
#include <StreamingAppOutput.hpp>

#include <TaskgraphParser.hpp>
#include <ArchitectureParser.hpp>
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
			parse(std::istream &taskgraph, std::istream &architecture, std::istream &schedule) const;

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
			std::vector<ArchitectureParser*> architectureParsers;
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

