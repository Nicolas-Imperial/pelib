#ifndef MAKESPAN_CALCULATOR
#define MAKESPAN_CALCULATOR
#include "Record.hpp"

namespace pelib
{
  class MakespanCalculator
  {
  public:
    virtual ~MakespanCalculator(){}

    virtual double calculate(const Record& tasks,const Record& architecture) const = 0;
  };
}



#endif
