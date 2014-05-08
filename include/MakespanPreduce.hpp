#ifndef MAKESPAN_PREDUCE
#define MAKESPAN_PREDUCE

#include "MakespanCalculator.hpp"
#include "Record.hpp"

namespace pelib
{
  class MakespanPreduce : public MakespanCalculator
  {
  public:


    double calculate(const Record& tasks,const Record& architecture) const;
  };
}



#endif
