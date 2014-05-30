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

	virtual double calculate(const Taskgraph& tg,const Architecture& arch) const;
 
  private:
    const double value;
  };
}



#endif
