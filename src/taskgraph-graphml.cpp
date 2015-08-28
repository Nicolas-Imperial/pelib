#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/GraphML.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

// /!\ the content of argv is freed after this function is run
pelib::Record*
pelib_parse(std::istream& cin, size_t argc, char **argv)
{
// We don't care about any argument here
#if 0
	while(*argv != NULL)
	{
		cout << *argv << " ";
		argv++;
	}
	cout << endl;
#endif

//	cout << cin.rdbuf();
	return GraphML().parse(cin);
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Taskgraph* tg = (Taskgraph*)records.find(typeid(Taskgraph).name())->second;
	Platform* pf = (Platform*)records.find(typeid(Platform).name())->second;

	if(records.find(typeid(Platform).name()) != records.end())
	{
		GraphML().dump(cout, tg, pf);
	}
	else
	{
		GraphML().dump(cout, tg);
	}
	//debug("hello world");
}

#ifdef __cplusplus
}
#endif

