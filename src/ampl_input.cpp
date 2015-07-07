#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/AmplInput.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

typedef struct
{
	vector<string> include;
	vector<string> exclude;
} filter_t;

static filter_t
parse_args(char **argv)
{
	filter_t filter;
	while((void*)argv[0] != NULL)
	{
		if(string(*argv).compare("--include") == 0)
		{
			while(string(*argv).compare("--") != 0 && (void*)argv != NULL)
			{
				filter.include.insert(filter.include.begin(), string(*argv));
				argv++;
			}
		}

		if(string(*argv).compare("--include") == 0)
		{
			while(string(*argv).compare("--") != 0 && (void*)argv != NULL)
			{
				filter.include.insert(filter.exclude.begin(), string(*argv));
			}
		}
	}

	return filter;
}

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
	Algebra al = AmplInput(AmplInput::intFloatHandlers()).parse(cin);
	return new Algebra(al);
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	Algebra al = *(Algebra*)(records.find(typeid(Algebra).name())->second);
	AmplInput(AmplInput::intFloatHandlers()).dump(cout, al);
}

#ifdef __cplusplus
}
#endif

