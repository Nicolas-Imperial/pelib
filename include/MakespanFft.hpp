#ifndef MAKESPAN_FFT
#define MAKESPAN_FFT

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanFft : public MakespanCalculator
  {
  public:


    double calculate(const Record& tasks,const Record& architecture) const;
  };
}



#endif
