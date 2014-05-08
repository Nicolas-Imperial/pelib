#include "MakespanNumeric.hpp"

using namespace pelib;
using namespace std;

MakespanNumeric::MakespanNumeric(double val) : value(val) {}

double MakespanNumeric::calculate(const Record& tasks,const Record& architecture) const
{
  return value;
}
