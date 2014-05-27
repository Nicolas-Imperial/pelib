#include <MakespanCalculator.hpp>

#include <MakespanSynthetic.hpp>
#include <MakespanFFT.hpp>
#include <MakespanPreduce.hpp>
#include <MakespanMergesort.hpp>
#include <MakespanConstant.hpp>

namespace pelib
{
	const MakespanCalculator*
	MakespanCalculator::getMakespanCalculator(const std::string key)
	{
		if(key.compare("fml:synthetic") == 0)
			return new MakespanSynthetic();
		if(key.compare("fml:fft") == 0)
			return new MakespanFFT();
		if(key.compare("fml:preduce") == 0)
			return new MakespanPreduce();
		if(key.compare("fml:mergesort") == 0)
			return new MakespanMergesort();
     
	// We don't want to compute anything, just get a calculator for the string
	/*
	try{
		//Maybe it is just a number, already calculated. Fine
		return new MakespanConstant(boost::lexical_cast<float>(key));
		std::cout << "FLOAT!";
	}
	catch(const boost::bad_lexical_cast &)
	{
	}
        */
		throw std::runtime_error("Makespan type not recognized: " + key);
	}
}
