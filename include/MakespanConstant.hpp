#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_CONSTANT
#define MAKESPAN_CONSTANT

namespace pelib
{
  class MakespanConstant : public MakespanCalculator
  {
  public:
    MakespanConstant(double val);

    double calculate(const Algebra& tasks,const Algebra& architecture) const;
 
  private:
    const double value;
  };
}



#endif
