#include <StreamingAppData.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{
	class Schedule : public StreamingAppData
	{
		public:
			virtual Schedule* clone() const;

		protected:
		private:		
	};
}

#endif
