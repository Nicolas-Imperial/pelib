#ifndef MAKESPAN_NUMERIC
#define MAKESPAN_NUMERIC

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanNumeric : public MakespanCalculator
  {
  public:
    MakespanNumeric(double val);

    double calculate(const Record& tasks,const Record& architecture) const;
 
  private:
    const double value;
  };
}



#endif
