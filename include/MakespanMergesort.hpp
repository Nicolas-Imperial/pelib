#ifndef MAKESPAN_MERGESORT
#define MAKESPAN_MERGESORT

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanMergesort : public MakespanCalculator
  {
  public:


    double calculate(const Record& tasks,const Record& architecture) const;
  };
}



#endif
