#ifndef MAKESPAN_RANDOM
#define MAKESPAN_RANDOM

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanRandom : public MakespanCalculator
  {
  public:


    double calculate(const Record& tasks,const Record& architecture) const;
  };
}



#endif
