#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <MakespanCalculator.hpp>
#include <MakespanSynthetic.hpp>
#include <MakespanFFT.hpp>
#include <MakespanPreduce.hpp>
#include <MakespanMergesort.hpp>
#include <MakespanConstant.hpp>
#include <MakespanFormula.hpp>
#include <ParseException.hpp>

using namespace boost::algorithm;

namespace pelib
{
	MakespanCalculator*
	MakespanCalculator::getMakespanCalculator(const std::string str)
	{
		string key(str);
		trim(key);

		try{
			if(key.substr(0, 4) == "fml:")
			{
				return new MakespanFormula(key.substr(4));
			}
			else
			{
				//Maybe it is just a number, already calculated. Fine
				return new MakespanConstant(boost::lexical_cast<float>(key));
			}
		}
		catch(const boost::bad_lexical_cast &)
		{
			throw ParseException("Makespan type not recognized: " + key);
		}
	}
}
