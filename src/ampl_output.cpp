#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/AmplOutput.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

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
	Algebra al = AmplOutput(AmplOutput::intFloatHandlers()).parse(cin);
	return new Algebra(al);
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Algebra al = *(Algebra*)(records.find(typeid(Algebra).name())->second);
	AmplOutput(AmplOutput::intFloatHandlers()).dump(cout, al);
}

#ifdef __cplusplus
}
#endif

