#include <boost/lexical_cast.hpp>

#include <MakespanCalculator.hpp>
#include <MakespanSynthetic.hpp>
#include <MakespanFFT.hpp>
#include <MakespanPreduce.hpp>
#include <MakespanMergesort.hpp>
#include <MakespanConstant.hpp>
#include <ParseException.hpp>

namespace pelib
{
	MakespanCalculator*
	MakespanCalculator::getMakespanCalculator(const std::string key)
	{
		if(key.compare("class:synthetic") == 0)
			return new MakespanSynthetic();
		if(key.compare("class:fft") == 0)
			return new MakespanFFT();
		if(key.compare("class:preduce") == 0)
			return new MakespanPreduce();
		if(key.compare("class:mergesort") == 0)
			return new MakespanMergesort();
     
		try{
			//Maybe it is just a number, already calculated. Fine
			return new MakespanConstant(boost::lexical_cast<float>(key));
		}
		catch(const boost::bad_lexical_cast &)
		{
			throw ParseException("Makespan type not recognized: " + key);
		}
	}
}
