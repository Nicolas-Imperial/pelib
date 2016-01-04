/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <pelib/Platform.hpp>
#include <pelib/Core.hpp>
#include <pelib/DummyCore.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Set.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>

using namespace std;

namespace pelib
{
	Platform::Platform()
	{
		set<float> f;
		f.insert(1);
		Core *core = new DummyCore(f, 1);
		this->cores.insert(core);
		set<const Core*> island;
		island.insert(core);
		main.insert(set<const Core*>(island));
		shared.insert(set<const Core*>(island));
		priv.insert(set<const Core*>(island));
		voltage.insert(set<const Core*>(island));
		freq.insert(set<const Core*>(island));
	}

	Platform::Platform(const set<const Core*>& cores)
	{
		//set<const Core*> shared;
		for(set<const Core*>::const_iterator i = cores.begin(); i != cores.end(); i++)
		{
			Core *core = (*i)->clone();
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);
			//shared.insert(core);

			// Each core is its own Platform::island of anything
			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
		}
		//this->shared.insert(set<const Core*>(shared));
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
		//set<const Core*> shared;
		for(size_t i = 0; i < p; i++)
		{
			Core *core = ref->clone();
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);
			//shared.insert(core);

			// Each core is its own Platform::island of anything
			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
		}
		//this->shared.insert(shared);
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
			set<const Core*> island;
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
		const Scalar<float> *f_unit = arch.find<Scalar<float> >("Funit");
		const Set<float> *set_F = arch.find<Set<float> >("F");

		if(scalar_p == NULL || set_F == NULL)
		{
			throw ParseException(std::string("Missing core number scalar \"p\" or frequency set \"F\" in input."));
		}

		//set<const Core*> shared;
		for(size_t i = 0; i < scalar_p->getValue(); i++)
		{
			const Core *core = new DummyCore(set_F->getValues(), f_unit == NULL ? 1 : f_unit->getValue());
			this->cores.insert(core);
			set<const Core*> island;
			island.insert(core);

			this->shared.insert(set<const Core*>(island));
			this->main.insert(set<const Core*>(island));
			this->priv.insert(set<const Core*>(island));
			this->voltage.insert(set<const Core*>(island));
			this->freq.insert(set<const Core*>(island));
		}
		//this->shared.insert(shared);
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
		Scalar<float> f_unit("Funit", (*this->getCores().begin())->getFrequencyUnit());
		Set<float> set_F("F", (*this->getCores().begin())->getFrequencies());

		record.insert(&scalar_p);
		record.insert(&f_unit);
		record.insert(&set_F);

		return record;
	}

	const Core*
	Platform::getCore(size_t id) const
	{
		if(id > this->getCores().size())
		{
			throw CastException("Requesting core beyond the number of core in the platform.");
		}
		std::set<const Core*>::iterator it = this->cores.begin();
		std::advance(it, id - 1);
		return *it;
	}

	size_t
	Platform::getCoreId(const Core* core) const
	{
		if(this->getCores().find(core) == this->getCores().end())
		{
			throw CastException("Core is not part of the platform.");
		}

		return std::distance(this->cores.begin(), this->cores.find(core)) + 1;
	}

	const std::set<Platform::island>&
	Platform::getSharedMemoryIslands() const
	{
		return this->shared;
	}

	const std::set<Platform::island>
	Platform::getSharedMemoryIslands(size_t id) const
	{
		if(id > this->getCores().size())
		{
			throw CastException("Requesting shared memory island beyond the number of islands in the platform.");
		}
		set<Platform::island> islands;
		const Core* core = this->getCore(id);
		for(set<Platform::island >::const_iterator i = this->getSharedMemoryIslands().begin(); i != this->getSharedMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				islands.insert(*i);
			}
		}

		return islands;
	}

	const std::set<int>
	Platform::getSharedMemoryIslands(const std::set<Platform::island>& islands) const
	{
		set<int> indexes;
		for(set<Platform::island>::const_iterator i = islands.begin(); i != islands.end(); i++)
		{
			if(this->getSharedMemoryIslands().find(*i) != this->getSharedMemoryIslands().end())
			{
				indexes.insert((int)std::distance(this->getSharedMemoryIslands().begin(), this->getSharedMemoryIslands().find(*i)) + 1);
			}
			else
			{
				throw CastException("Could not find island in platform.");
			}
		}

		return indexes;
	}

	const std::set<Platform::island>
	Platform::getSharedMemoryIslands(const std::set<int>& islands) const
	{
		std::set<island> out;
		for(std::set<int>::const_iterator i = islands.begin(); i != islands.end(); i++)
		{
			set<island>::const_iterator ii = this->getSharedMemoryIslands().begin();
			if(*i > (ptrdiff_t)this->getSharedMemoryIslands().size())
			{
				throw CastException("Trying to get shared memory island beyond the capacity of the platform.");
			}
			std::advance(ii, *i);
			out.insert(*ii);
		} 

		return out;
	}

	const std::set<Platform::island>&
	Platform::getMainMemoryIslands() const
	{
		return this->main;
	}

	const std::set<Platform::island>
	Platform::getMainMemoryIslands(size_t id) const
	{
		set<Platform::island> islands;
		const Core* core = this->getCore(id);
		for(set<Platform::island>::const_iterator i = this->getMainMemoryIslands().begin(); i != this->getMainMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				islands.insert(*i);
			}
		}

		return islands;
	}

	const std::set<Platform::island>&
	Platform::getPrivateMemoryIslands() const
	{
		return this->priv;
	}

	const std::set<Platform::island>
	Platform::getPrivateMemoryIslands(size_t id) const
	{
		set<Platform::island> islands;
		const Core* core = this->getCore(id);
		for(set<Platform::island>::const_iterator i = this->getPrivateMemoryIslands().begin(); i != this->getPrivateMemoryIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				islands.insert(*i);
			}
		}

		return islands;
	}

	const std::set<Platform::island>&
	Platform::getVoltageIslands() const
	{
		return this->voltage;
	}

	const Platform::island&
	Platform::getVoltageIsland(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<Platform::island>::const_iterator i = this->getVoltageIslands().begin(); i != this->getVoltageIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any Platform::island of the platform.");
	}

	const std::set<Platform::island>&
	Platform::getFrequencyIslands() const
	{
		return this->freq;
	}

	const Platform::island&
	Platform::getFrequencyIsland(size_t id) const
	{
		const Core* core = this->getCore(id);
		for(set<Platform::island>::const_iterator i = this->getFrequencyIslands().begin(); i != this->getFrequencyIslands().end(); i++)
		{
			if(i->find(core) != i->end())
			{
				return *i;
			}
		}

		throw CastException("Cannot find core in any Platform::island of the platform.");
	}
}
