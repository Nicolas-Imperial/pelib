#include <Record.hpp>
#include <DataOutput.hpp>
#include <Scalar.hpp>
#include <Data.hpp>

#ifndef PELIB_RECORDOUTPUT
#define PELIB_RECORDOUTPUT

namespace pelib
{
	class RecordOutput
	{
		public:
			RecordOutput();

			virtual
			~RecordOutput();

			virtual
			std::ostream&
			dump(std::ostream& o, const Record &record) const = 0;
			
		protected:
		private:
	};
}

#endif