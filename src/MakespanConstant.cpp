#include "MakespanConstant.hpp"

using namespace pelib;
using namespace std;

MakespanConstant::MakespanConstant(double val) : value(val) {}

double MakespanConstant::calculate(const Algebra& tasks,const Algebra&  architecture) const
{
  return value;
}
