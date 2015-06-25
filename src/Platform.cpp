#include <Platform.hpp>

#include <Scalar.hpp>
#include <Set.hpp>

using namespace std;

namespace pelib
{
	Platform::Platform()
	{
		coreNumber = 1;
		frequencies.insert(1);
	}
	
	Platform::Platform(const Platform *arch)
	{
		coreNumber = arch->getCoreNumber();
		frequencies = arch->getFrequencies();
	}

	Platform::~Platform()
	{
		// Do nothing
	}
	
	Platform*
	Platform::clone() const
	{
		Platform *arch = new Platform();
		arch->setCoreNumber(this->getCoreNumber());
		arch->setFrequencies(this->getFrequencies());

		return arch;
	}

	int
	Platform::getCoreNumber() const
	{
		return this->coreNumber;
	}
	
	void
	Platform::setCoreNumber(int p)
	{
		this->coreNumber = p;
	}

	const std::set<float>&
	Platform::getFrequencies() const
	{
		return this->frequencies;
	}
		
	void
	Platform::setFrequencies(const std::set<float>& freq)
	{
		this->frequencies = freq;
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
