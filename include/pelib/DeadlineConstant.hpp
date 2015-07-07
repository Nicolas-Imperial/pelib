#include <pelib/DeadlineCalculator.hpp>
#include <pelib/Algebra.hpp>

#ifndef DEADLINE_CONSTANT
#define DEADLINE_CONSTANT

namespace pelib
{
  class DeadlineConstant : public DeadlineCalculator
  {
  public:
    DeadlineConstant(double val);

	virtual double calculate(const Taskgraph& tg, const Platform& arch) const;
 
  private:
    const double value;
  };
}



#endif
