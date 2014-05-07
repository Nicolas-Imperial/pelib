#ifndef MAKESPANSELECTOR
#define MAKESPANSELECTOR

#include "MakespanCalculator.hpp"
#include "MakespanRandom.hpp"

namespace pelib{
  class MakespanSelector{
  public:
    MakespanSelector() = delete;
    static const MakespanCalculator* getMakespanCalculator(const std::string key)
    {
      if(key.compare("fml:random") == 0)
	return new MakespanRandom();
      else throw std::runtime_error("Makespan type not recognized: " + key);
    }
  };

}





#endif
