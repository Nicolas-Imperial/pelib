#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_MERGESORT
#define MAKESPAN_MERGESORT

namespace pelib
{
  class MakespanMergesort : public MakespanCalculator
  {
  public:
    virtual double calculate(const Taskgraph& tg,const Platform& arch) const;
  };
}



#endif
