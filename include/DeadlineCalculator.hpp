#include <Platform.hpp>
#include <Taskgraph.hpp>

#ifndef DEADLINE_CALCULATOR
#define DEADLINE_CALCULATOR

namespace pelib
{
  class DeadlineCalculator
  {
	public:
    virtual ~DeadlineCalculator(){}
    virtual double calculate(const Taskgraph& tg, const Platform& arch) const = 0;

    static DeadlineCalculator* getDeadlineCalculator(const std::string key);
  };
}



#endif
