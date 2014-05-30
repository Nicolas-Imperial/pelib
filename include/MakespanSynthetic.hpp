#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_SYNTHETIC
#define MAKESPAN_SYNTHETIC

namespace pelib
{
  class MakespanSynthetic : public MakespanCalculator
  {
  public:
    virtual double calculate(const Taskgraph& tg,const Architecture& arch) const;
  };
}



#endif
