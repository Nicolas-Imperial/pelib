#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <pelib/DeadlineCalculator.hpp>
#include <pelib/DeadlineConstant.hpp>
#include <pelib/DeadlineFormula.hpp>
#include <pelib/ParseException.hpp>

using namespace boost::algorithm;

namespace pelib
{
	DeadlineCalculator*
	DeadlineCalculator::getDeadlineCalculator(const std::string str)
	{
		string key(str);
		trim(key);

		try{
			if(key.find("exprtk") == 0)
			{
				return new DeadlineFormula(key.substr(string("exprtk").size() + 1));
			}
			else
			{
				//Maybe it is just a number, already calculated. Fine
				return new DeadlineConstant(boost::lexical_cast<float>(key));
			}
		}
		catch(const boost::bad_lexical_cast &)
		{
			throw ParseException("Deadline type not recognized: " + key);
		}
	}
}
