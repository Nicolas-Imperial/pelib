#include <DummyCore.hpp>

using namespace std;

namespace pelib
{
	DummyCore::DummyCore(set<float> f)
	{
		this->frequencies = f;
	}

	DummyCore::DummyCore(const DummyCore* src)
	{
		this->frequencies == src->getFrequencies();
	}

	DummyCore::~DummyCore()
	{
		// Do nothing
	}

	Core*
	DummyCore::clone() const
	{
		return new DummyCore(this);
	}

	const set<float>&
	DummyCore::getFrequencies() const
	{
		return this->frequencies;
	}
}
