#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <amplrecord.hpp>

namespace pelib
{
	boost::cmatch
	AmplRecord::match(std::string regex, std::string str)
	{
		boost::cmatch match;
		boost::regex param_scalar(regex);

		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			return match;
		}
		else
		{
			throw ParseException("Text doesn't match regex");
		}
	}

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
