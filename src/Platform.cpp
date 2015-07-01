#include <Platform.hpp>
#include <Core.hpp>
#include <DummyCore.hpp>

#include <Scalar.hpp>
#include <Set.hpp>
#include <ParseException.hpp>
#include <CastException.hpp>

using namespace std;

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

namespace pelib
{
	Platform::Platform()
	{
		// Do nothing and leave all collections empty
	}

	Platform::Platform(const set<const Core*>& cores)
	{
		for(set<const Core*>::const_iterator i = cores.begin(); i != cores.end(); i++)
		{
			Core *core = (*i)->clone();
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);

			// Each core is its own island of anything
			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
		}
	}
	
	Platform::Platform(const std::set<const Core*>& cores, const std::set<std::set <const Core*> >& shared, const std::set<std::set <const Core*> >& main, const std::set<std::set <const Core*> >& priv, const std::set<std::set <const Core*> >& voltage, const std::set<std::set <const Core*> >& freq)
	{
		this->cores = cores;
		this->shared = shared;
		this->main = main;
		this->priv = priv;
		this->voltage = voltage;
		this->freq = freq;
	}

	Platform::Platform(size_t p, const Core* ref)
	{
		for(size_t i = 0; i < p; i++)
		{
			Core *core = ref->clone();
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);

			// Each core is its own island of anything
			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
		}
	}

	Platform::Platform(const Platform *arch)
	{
		for(set<const Core*>::const_iterator i = arch->getCores().begin(); i != arch->getCores().end(); i++)
		{
			this->cores.insert((*i)->clone());
		}

		this->shared.clear();
		this->main.clear();
		this->priv.clear();
		this->voltage.clear();
		this->freq.clear();

		for(set<set<const Core*> >::const_iterator i = arch->getSharedMemoryIslands().begin(); i != arch->getSharedMemoryIslands().end(); i++)
		{
			set<const Core*>  island;
			for(set<const Core*>::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island.insert(this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->shared.insert(island);
		}

		for(set<set<const Core*> >::const_iterator i = arch->getMainMemoryIslands().begin(); i != arch->getMainMemoryIslands().end(); i++)
		{
			set<const Core*>  island;
			for(set<const Core*>::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island.insert(this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->main.insert(island);
		}

		for(set<set<const Core*> >::const_iterator i = arch->getPrivateMemoryIslands().begin(); i != arch->getPrivateMemoryIslands().end(); i++)
		{
			set<const Core*>  island;
			for(set<const Core*>::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island.insert(this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->priv.insert(island);
		}

		for(set<set<const Core*> >::const_iterator i = arch->getVoltageIslands().begin(); i != arch->getVoltageIslands().end(); i++)
		{
			set<const Core*>  island;
			for(set<const Core*>::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island.insert(this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->voltage.insert(island);
		}

		for(set<set<const Core*> >::const_iterator i = arch->getFrequencyIslands().begin(); i != arch->getFrequencyIslands().end(); i++)
		{
			set<const Core*>  island;
			for(set<const Core*>::const_iterator j = i->begin(); j != i->end(); j++)
			{
				island.insert(this->getCore(arch->getCoreId(*arch->getCores().find(*j))));
			}

			this->freq.insert(island);
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
			const Core *core = new DummyCore(set_F->getValues());
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);

			// Each core is its own island of anything
			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
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

	const Core*
	Platform::getCore(size_t id) const
	{
		std::set<const Core*>::iterator it = this->cores.begin();
		std::advance(it, id - 1);
		return *it;
	}

	size_t
	Platform::getCoreId(const Core* core) const
	{
		return std::distance(this->cores.begin(), this->cores.find(core)) + 1;
	}

	const std::set<std::set<const Core*> >&
	Platform::getSharedMemoryIslands() const
	{
		return this->shared;
	}

	const std::set<const Core*>&
	Platform::getSharedMemoryIslands(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<set<const Core*> >::const_iterator i = this->getSharedMemoryIslands().begin(); i != this->getSharedMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any island of the platform.");
	}

	const std::set<std::set<const Core*> >&
	Platform::getMainMemoryIslands() const
	{
		return this->main;
	}

	const std::set<const Core*>&
	Platform::getMainMemoryIslands(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<set<const Core*> >::const_iterator i = this->getMainMemoryIslands().begin(); i != this->getMainMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any island of the platform.");
	}

	const std::set<std::set<const Core*> >&
	Platform::getPrivateMemoryIslands() const
	{
		return this->priv;
	}

	const std::set<const Core*>&
	Platform::getPrivateMemoryIslands(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<set<const Core*> >::const_iterator i = this->getPrivateMemoryIslands().begin(); i != this->getPrivateMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any island of the platform.");
	}

	const std::set<std::set<const Core*> >&
	Platform::getVoltageIslands() const
	{
		return this->voltage;
	}

	const std::set<const Core*>&
	Platform::getVoltageIslands(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<set<const Core*> >::const_iterator i = this->getVoltageIslands().begin(); i != this->getVoltageIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any island of the platform.");
	}

	const std::set<std::set<const Core*> >&
	Platform::getFrequencyIslands() const
	{
		return this->freq;
	}

	const std::set<const Core*>&
	Platform::getFrequencyIslands(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<set<const Core*> >::const_iterator i = this->getFrequencyIslands().begin(); i != this->getFrequencyIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any island of the platform.");
	}
}
