#include <Platform.hpp>

#include <Scalar.hpp>
#include <Set.hpp>
#include <ParseException.hpp>

using namespace std;

namespace pelib
{
	Platform::Platform(size_t p, const set<float> &f)
	{
		coreNumber = p;
		frequencies = f;
	}
	
	Platform::Platform(const Platform *arch)
	{
		coreNumber = arch->getCoreNumber();
		frequencies = arch->getFrequencies();
	}

	Platform::Platform(const Algebra &arch)
	{
		const Scalar<float> *scalar_p = arch.find<Scalar<float> >("p");
		const Set<float> *set_F = arch.find<Set<float> >("F");

		if(scalar_p == NULL || set_F == NULL)
		{
			throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
		}
	
		this->coreNumber = (size_t)scalar_p->getValue();
		this->frequencies = set_F->getValues();
	}

	Platform::~Platform()
	{
		// Do nothing
	}
	
	Platform*
	Platform::clone() const
	{
		return new Platform(this->getCoreNumber(), this->getFrequencies());
	}

	int
	Platform::getCoreNumber() const
	{
		return this->coreNumber;
	}
	
	const std::set<float>&
	Platform::getFrequencies() const
	{
		return this->frequencies;
	}
		
	Algebra
	Platform::buildAlgebra() const
	{
		Algebra record;

		Scalar<float> scalar_p("p", coreNumber);
		Set<float> set_F("F", frequencies);

		record.insert(&scalar_p);
		record.insert(&set_F);

		return record;
	}
}
