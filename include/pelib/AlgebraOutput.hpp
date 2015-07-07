#include <pelib/Algebra.hpp>
#include <pelib/DataOutput.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Data.hpp>

#include <pelib/Algebra.hpp>
#include <pelib/RecordOutput.hpp>

#ifndef PELIB_ALGEBRAOUTPUT
#define PELIB_ALGEBRAOUTPUT

namespace pelib
{
	class AlgebraOutput : public RecordOutput
	{
		public:
			virtual
			void
			dump(std::ostream& o, const Algebra &record) const = 0;

			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const = 0;
			
		protected:
		private:
	};
}

#endif
