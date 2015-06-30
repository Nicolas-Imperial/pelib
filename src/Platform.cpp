#include <Platform.hpp>
#include <Core.hpp>
#include <DummyCore.hpp>

#include <Scalar.hpp>
#include <Set.hpp>
#include <ParseException.hpp>
#include <CastException.hpp>

using namespace std;

namespace pelib
{
	Platform::Platform(const set<const Core*>& cores)
	{
		for(set<const Core*>::const_iterator i = cores.begin(); i != cores.end(); i++)
		{
			this->cores.insert((*i)->clone());
		}
	}
	
	Platform::Platform(const Platform *arch)
	{
		for(set<const Core*>::const_iterator i = arch->getCores().begin(); i != arch->getCores().end(); i++)
		{
			this->cores.insert((*i)->clone());
		}
	}

	Platform::Platform(const Algebra &arch)
	{
		const Scalar<float> *scalar_p = arch.find<Scalar<float> >("p");
		const Set<float> *set_F = arch.find<Set<float> >("F");

		if(scalar_p == NULL || set_F == NULL)
		{
			throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
		}

		for(size_t i = 0; i < scalar_p->getValue(); i++)
		{
			this->cores.insert(new DummyCore(set_F->getValues()));
		}
	}

	Platform::~Platform()
	{
		for(set<const Core*>::const_iterator i = this->getCores().begin(); i != this->getCores().end(); i++)
		{
			delete *i;
		}
	}
	
	Platform*
	Platform::clone() const
	{
		return new Platform(this);
	}

	const std::set<const Core*>&
	Platform::getCores() const
	{
		return this->cores;
	}
		
	bool
	Platform::isHomogeneous() const
	{
		const char* type = NULL;
		
		this->cores.size() == 1;
		for(set<const Core*>::const_iterator i = this->getCores().begin(); i != this->getCores().end(); i++)
		{
			if(type != NULL)
			{
				if(string(type).compare(typeid(**i).name()) != 0)
				{
					return false;
				}
			}

			type = typeid(**i).name();
		}

		// If we reach this point having read at least one string, then this is homogeneous
		return type != NULL;
	}

	Algebra
	Platform::buildAlgebra() const
	{
		Algebra record;

		if(!this->isHomogeneous())
		{
			throw CastException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
		}

		Scalar<float> scalar_p("p", this->getCores().size());
		Set<float> set_F("F", (*this->getCores().begin())->getFrequencies());

		record.insert(&scalar_p);
		record.insert(&set_F);

		return record;
	}
}
