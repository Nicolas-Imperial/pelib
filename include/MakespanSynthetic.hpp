#ifndef MAKESPAN_SYNTHETIC
#define MAKESPAN_SYNTHETIC

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanSynthetic : public MakespanCalculator
  {
  public:


    double calculate(const Record& tasks,const Record& architecture) const;
  };
}



#endif
