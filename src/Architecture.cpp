#include <Architecture.hpp>

#include <Scalar.hpp>
#include <Set.hpp>

using namespace std;

namespace pelib
{
	Architecture::Architecture()
	{
		coreNumber = 1;
		frequencies.insert(1);
	}
	
	Architecture::Architecture(const Architecture *arch)
	{
		coreNumber = arch->getCoreNumber();
		frequencies = arch->getFrequencies();
	}
	
	Architecture*
	Architecture::clone() const
	{
		Architecture *arch = new Architecture();
		arch->setCoreNumber(this->getCoreNumber());
		arch->setFrequencies(this->getFrequencies());

		return arch;
	}

	int
	Architecture::getCoreNumber() const
	{
		return this->coreNumber;
	}
	
	void
	Architecture::setCoreNumber(int p)
	{
		this->coreNumber = p;
	}

	const std::set<int, std::less<int>, std::allocator<int> >&
	Architecture::getFrequencies() const
	{
		return this->frequencies;
	}
		
	void
	Architecture::setFrequencies(const std::set<int, std::less<int>, std::allocator<int> >& freq)
	{
		this->frequencies = freq;
	}
	
	Algebra
	Architecture::buildAlgebra() const
	{
		Algebra record;

		Scalar<int> scalar_p("p", coreNumber);
		Set<int> set_F("F", frequencies);

		record.insert(&scalar_p);
		record.insert(&set_F);

		return record;
	}
}
