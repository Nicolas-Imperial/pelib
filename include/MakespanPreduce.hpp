#include "MakespanCalculator.hpp"
#include "Algebra.hpp"

#ifndef MAKESPAN_PREDUCE
#define MAKESPAN_PREDUCE

namespace pelib
{
  class MakespanPreduce : public MakespanCalculator
  {
  public:
	virtual double calculate(const Taskgraph& tg,const Architecture& arch) const;
  };
}



#endif
