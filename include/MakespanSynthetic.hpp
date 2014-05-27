#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_SYNTHETIC
#define MAKESPAN_SYNTHETIC

namespace pelib
{
  class MakespanSynthetic : public MakespanCalculator
  {
  public:
    double calculate(const Algebra& tasks,const Algebra& architecture) const;
  };
}



#endif
