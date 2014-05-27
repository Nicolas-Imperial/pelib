#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_FFT
#define MAKESPAN_FFT

namespace pelib
{
  class MakespanFFT : public MakespanCalculator
  {
  public:


    double calculate(const Algebra& tasks,const Algebra& architecture) const;
  };
}



#endif
