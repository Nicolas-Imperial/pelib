#include <RecordOutput.hpp>
#include <StreamingApp.hpp>

#include <Schedule.hpp>

#ifndef PELIB_STREAMINGAPPOUTPUT
#define PELIB_STREAMINGAPPOUTPUT

namespace pelib
{
	class StreamingAppOutput : public RecordOutput
	{
		public:
			virtual
			void
			dump(std::ostream& o, const StreamingApp &record) const = 0;

			virtual
			void
			dumpSchedule(std::ostream& o, const Schedule *data) const = 0;
			
		protected:
		private:
	};
}

#endif
