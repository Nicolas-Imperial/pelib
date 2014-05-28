#include <StreamingAppData.hpp>
#include <Algebra.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{
	class Schedule : public StreamingAppData
	{
		public:
			virtual Schedule* clone() const;
			virtual Algebra* buildAlgebra() const;

		protected:
		private:		
	};
}

#endif
