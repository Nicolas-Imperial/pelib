#include <sstream>

#ifndef PELIB_RECORDPARSER
#define PELIB_RECORDPARSER

namespace pelib
{
	class RecordParser
	{
		public:
			std::istream& getEmptyStream();
			virtual ~RecordParser();
		protected:
			std::istringstream emptyStream;
		private:
	};
}

#endif
