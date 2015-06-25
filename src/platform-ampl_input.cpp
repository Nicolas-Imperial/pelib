#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <AmplPlatform.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
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
	return AmplPlatform().parse(cin);
}

void
pelib_dump(std::ostream& out, std::map<const char*, Record*> records, size_t argc, char **argv)
{
}

#ifdef __cplusplus
}
#endif

