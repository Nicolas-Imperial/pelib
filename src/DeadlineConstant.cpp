#include <pelib/DeadlineConstant.hpp>

using namespace pelib;
using namespace std;

DeadlineConstant::DeadlineConstant(double val) : value(val) {}

double DeadlineConstant::calculate(const Taskgraph& tg,const Platform& arch) const
{
  return value;
}
