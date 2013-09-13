#include <AmplRecord.hpp>

#ifndef AMPLRECORDOUTPUT
#define AMPLRECORDOUTPUT

namespace pelib
{
	class AmplRecordOutput: public AmplRecord
	{
		// AmplRecordOutput lies for the sake of having a root class for AmplOutput classes
		public:
		virtual void
		setName(std::string name) = 0;

		protected:
		private:
	};
}

#endif
