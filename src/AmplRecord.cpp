#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplRecord.hpp>

namespace pelib
{
	AmplRecord::~AmplRecord() {}
	std::string
	AmplRecord::getName() const
	{
		return name;
	}

	std::ostream&
	operator<<(std::ostream& o, const AmplRecord& r)
	{
		return r.dump(o);
	}
}
