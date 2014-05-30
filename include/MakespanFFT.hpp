#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_FFT
#define MAKESPAN_FFT

namespace pelib
{
  class MakespanFFT : public MakespanCalculator
  {
  public:
    virtual double calculate(const Taskgraph& tg,const Architecture& arch) const;
  };
}



#endif
