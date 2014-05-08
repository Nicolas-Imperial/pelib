#ifndef MAKESPANSELECTOR
#define MAKESPANSELECTOR

#include "MakespanCalculator.hpp"
#include "MakespanRandom.hpp"
#include "MakespanNumeric.hpp"
#include "MakespanFft.hpp"
#include "MakespanPreduce.hpp"
#include "MakespanMergesort.hpp"

#include <boost/lexical_cast.hpp>

namespace pelib{
  class MakespanSelector{
  public:
    MakespanSelector() = delete;
    static const MakespanCalculator* getMakespanCalculator(const std::string key)
    {
      if(key.compare("fml:random") == 0)
	return new MakespanRandom();
      if(key.compare("fml:fft") == 0)
	return new MakespanFft();
      if(key.compare("fml:preduce") == 0)
	return new MakespanPreduce();
      if(key.compare("fml.:mergesort") == 0)
	return new MakespanMergesort();
      
      try{
	//Maybe it is just a number, already calculated. Fine
	return new MakespanNumeric(boost::lexical_cast<float>(key));
	std::cout << "FLOAT!";
      }
      catch(const boost::bad_lexical_cast &){}

      //TODO: How to handle empty makespan speficier? Right not it is an error
      //TODO: it this desired behaviour?
      throw std::runtime_error("Makespan type not recognized: " + key);
    }
  };

}





#endif
