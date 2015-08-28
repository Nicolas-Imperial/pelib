#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/Set.hpp>
#include <pelib/AmplInput.hpp>
#include <pelib/Platform.hpp>

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
	AmplInput reader(AmplInput::floatHandlers());
	std::string line;
	Algebra alg_arch = reader.parse(cin);

	Platform *arch = new Platform(alg_arch);

	
	return arch;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	const Platform *arch = (Platform*)records.find(typeid(Platform).name())->second;

	AmplInput output(AmplInput::intFloatHandlers());
	Algebra alg = arch->buildAlgebra();
	output.dump(cout, alg);
}

#ifdef __cplusplus
}
#endif

