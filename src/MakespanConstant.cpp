#include "MakespanConstant.hpp"

using namespace pelib;
using namespace std;

MakespanConstant::MakespanConstant(double val) : value(val) {}

double MakespanConstant::calculate(const Taskgraph& tg,const Architecture& arch) const
{
  return value;
}
