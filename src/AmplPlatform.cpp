#include <AmplInput.hpp>

#include <Platform.hpp>
#include <AmplPlatform.hpp>
#include <ParseException.hpp>
#include <CastException.hpp>

#include <Scalar.hpp>
#include <Set.hpp>

using namespace pelib;
using namespace std;

void
AmplPlatform::dump(ostream& os, const StreamingAppData &data) const
{
	dump(os, &data);
}

void
AmplPlatform::dump(ostream& os, const StreamingAppData *data) const
{
	const Platform *arch = dynamic_cast<const Platform* >(data);
	if(arch == NULL) throw CastException("parameter \"data\" was not of type \"Platform*\".");

	AmplInput output(AmplInput::intFloatHandlers());
	Algebra alg = arch->buildAlgebra();
	output.dump(os, alg);
}

Platform*
AmplPlatform::parse(istream &is) const
{
	AmplInput reader(AmplInput::floatHandlers());
	std::string line;
	Algebra alg_arch = reader.parse(is);

	Platform *arch = new Platform();

	const Scalar<float> *scalar_p = alg_arch.find<Scalar<float> >("p");
	const Set<float> *set_F = alg_arch.find<Set<float> >("F");

	if(scalar_p == NULL || set_F == NULL)
	{
		throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
	}
	
	arch->setCoreNumber((int)scalar_p->getValue());
	arch->setFrequencies(set_F->getValues());
	
	return arch;
}

AmplPlatform*
AmplPlatform::clone() const
{
	return new AmplPlatform();
}
