#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/AmplInput.hpp>
#include <pelib/Taskgraph.hpp>

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
	Algebra data = AmplInput(AmplInput::floatHandlers()).parse(cin);
	return new Taskgraph(data);
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Taskgraph* tg = (Taskgraph*)records.find(typeid(Taskgraph).name())->second;
	Platform* pf = (Platform*)records.find(typeid(Platform).name())->second;

	if(records.find(typeid(Platform).name()) != records.end())
	{
		AmplInput(AmplInput::floatHandlers()).dump(cout, tg->buildAlgebra(*pf));
	}
	else
	{
		AmplInput(AmplInput::floatHandlers()).dump(cout, tg->buildAlgebra());
	}
}

#ifdef __cplusplus
}
#endif

