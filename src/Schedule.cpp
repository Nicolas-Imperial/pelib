#include <Schedule.hpp>

using namespace std;

namespace pelib
{
	Schedule*
	Schedule::clone() const
	{
		return new Schedule();
	}
}
