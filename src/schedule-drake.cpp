#include <iostream>

#include <pelib/output.h>

#include <pelib/DrakeCSchedule.hpp>
#include <pelib/Schedule.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Schedule *sc = (Schedule*)records.find(typeid(Schedule).name())->second;
	Taskgraph *tg = (Taskgraph*)records.find(typeid(Taskgraph).name())->second;
	Platform *pt = (Platform*)records.find(typeid(Platform).name())->second;
	DrakeCSchedule().dump(cout, sc, tg, pt);
}

#ifdef __cplusplus
}
#endif

