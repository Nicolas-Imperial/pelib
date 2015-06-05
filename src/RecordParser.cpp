#include <RecordParser.hpp>

using namespace std;

namespace pelib
{
	istream&
	RecordParser::getEmptyStream()
	{
		istringstream &local = emptyStream;
		return local;
	}

	RecordParser::~RecordParser()
	{
		/* Do nothing */
	}
}
