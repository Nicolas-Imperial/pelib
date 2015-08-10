#include <pelib/DummyCore.hpp>

using namespace std;

namespace pelib
{
	DummyCore::DummyCore(const set<float> &f)
	{
		this->frequencies = set<float>(f);
	}

	DummyCore::DummyCore(const DummyCore* src)
	{
		this->frequencies = set<float>(src->getFrequencies());
	}

	DummyCore::DummyCore(const DummyCore &src)
	{
		this->frequencies = set<float>(src.getFrequencies());
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
