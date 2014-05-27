#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_PREDUCE
#define MAKESPAN_PREDUCE

namespace pelib
{
  class MakespanPreduce : public MakespanCalculator
  {
  public:


    double calculate(const Algebra& tasks,const Algebra& architecture) const;
  };
}



#endif
