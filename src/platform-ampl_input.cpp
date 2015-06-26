#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <Set.hpp>
#include <AmplInput.hpp>
#include <Platform.hpp>

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

