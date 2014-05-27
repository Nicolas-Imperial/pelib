#include <Schedule.hpp>

using namespace std;

namespace pelib
{
	Schedule::Schedule(string name) : StreamingAppData(name) { /* Do nothing else */ };

	Schedule*
	Schedule::clone() const
	{
		return new Schedule(name);
	}
}
