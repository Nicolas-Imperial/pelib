#include <AmplInput.hpp>

#include <Architecture.hpp>
#include <AmplArchitecture.hpp>
#include <ParseException.hpp>
#include <CastException.hpp>

#include <Scalar.hpp>
#include <Set.hpp>

using namespace pelib;
using namespace std;

void
AmplArchitecture::dump(ostream& os, const StreamingAppData *data) const
{
	const Architecture *arch = dynamic_cast<const Architecture* >(data);
	if(arch == NULL) throw CastException("parameter \"data\" was not of type \"Architecture*\".");

	AmplInput output;
	Algebra *alg = arch->buildAlgebra();
	output.dump(os, *alg);
	delete alg;
}

Architecture*
AmplArchitecture::parse(istream &is)
{
	AmplInput reader;
	std::string line;
	Algebra alg_arch = reader.parse(is);

	Architecture *arch = new Architecture();

	const Scalar<int> *scalar_p = alg_arch.find<Scalar<int> >("p");
	const Set<int> *set_F = alg_arch.find<Set<int> >("F");

	if(scalar_p == NULL || set_F == NULL)
	{
		throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
	}
	
	arch->setCoreNumber(scalar_p->getValue());
	arch->setFrequencies(set_F->getValues());
	
	return arch;
}

AmplArchitecture*
AmplArchitecture::clone() const
{
	return new AmplArchitecture();
}
