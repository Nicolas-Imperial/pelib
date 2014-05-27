#include "Algebra.hpp"

#ifndef MAKESPAN_CALCULATOR
#define MAKESPAN_CALCULATOR

namespace pelib
{
  class MakespanCalculator
  {
  public:
    virtual ~MakespanCalculator(){}
    virtual double calculate(const Algebra& tasks,const Algebra& architecture) const = 0;

    static const MakespanCalculator* getMakespanCalculator(const std::string key);
  };
}



#endif
