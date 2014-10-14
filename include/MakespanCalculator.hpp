#include <Platform.hpp>
#include <Taskgraph.hpp>

#ifndef MAKESPAN_CALCULATOR
#define MAKESPAN_CALCULATOR

namespace pelib
{
  class MakespanCalculator
  {
	public:
    virtual ~MakespanCalculator(){}
    virtual double calculate(const Taskgraph& tg, const Platform& arch) const = 0;

    static MakespanCalculator* getMakespanCalculator(const std::string key);
  };
}



#endif
