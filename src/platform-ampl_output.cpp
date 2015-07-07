#include <iostream>

#include <pelib/parser.h>
#include <pelib/output.h>

#include <pelib/Set.hpp>
#include <pelib/DummyCore.hpp>
#include <pelib/AmplOutput.hpp>
#include <pelib/Platform.hpp>

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
	AmplOutput reader(AmplOutput::floatHandlers());
	std::string line;
	Algebra alg_arch = reader.parse(cin);


	const Scalar<float> *scalar_p = alg_arch.find<Scalar<float> >("p");
	const Set<float> *set_F = alg_arch.find<Set<float> >("F");

	if(scalar_p == NULL || set_F == NULL)
	{
		throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
	}

	set<const Core*> cores;
	for(size_t i = 0; i < (size_t)scalar_p->getValue(); i++)
	{
		cores.insert(new DummyCore(set_F->getValues()));
	}

	Platform *arch = new Platform(cores);
	
	return arch;
}

// /!\ the content of argv is freed after this function is run
void
pelib_dump(std::ostream& cout, std::map<const char*, Record*> records, size_t argc, char **argv)
{
	const Platform *arch = (Platform*)records.find(typeid(Platform).name())->second;

	AmplOutput output(AmplOutput::intFloatHandlers());
	Algebra alg = arch->buildAlgebra();
	output.dump(cout, alg);
}

#ifdef __cplusplus
}
#endif

