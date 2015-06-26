#include <iostream>

#include <pelib/output.h>

#include <DrakeCSchedule.hpp>
#include <Schedule.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Schedule *sc = (Schedule*)records.find(typeid(Schedule).name())->second;
	DrakeCSchedule().dump(cout, sc);
}

#ifdef __cplusplus
}
#endif

