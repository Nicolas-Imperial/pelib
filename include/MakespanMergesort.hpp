#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_MERGESORT
#define MAKESPAN_MERGESORT

namespace pelib
{
  class MakespanMergesort : public MakespanCalculator
  {
  public:


    double calculate(const Algebra& tasks,const Algebra& architecture) const;
  };
}



#endif
