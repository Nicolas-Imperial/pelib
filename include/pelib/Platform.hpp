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


#include <set>

#include <pelib/Core.hpp>
#include <pelib/Algebra.hpp>

#ifndef PELIB_PLATFORM
#define PELIB_PLATFORM

namespace pelib
{
	class Platform: public Record
	{
		public:
			typedef std::set<const Core*> island;

			Platform();
			Platform(size_t p, const Core* ref);
			Platform(const std::set<const Core*>&);
			Platform(const std::set<const Core*>&, const std::set<std::set <const Core*> >& shared, const std::set<std::set <const Core*> >& main, const std::set<std::set <const Core*> >& priv, const std::set<std::set <const Core*> >& voltage, const std::set<std::set <const Core*> >& freq);
			Platform(const Platform *arch);
			Platform(const Algebra &arch);
			virtual Platform* clone() const;
			virtual const std::set<const Core*>& getCores() const;
			virtual bool isHomogeneous() const;

			virtual const std::set<island>& getSharedMemoryIslands() const;
			virtual const std::set<island> getSharedMemoryIslands(size_t core_id) const;
			virtual const std::set<island> getSharedMemoryIslands(const std::set<int>& islands) const;
			virtual const std::set<int> getSharedMemoryIslands(const std::set<island>& islands) const;
			virtual const std::set<island>& getMainMemoryIslands() const;
			virtual const std::set<island> getMainMemoryIslands(size_t core_id) const;
			virtual const std::set<island>& getPrivateMemoryIslands() const;
			virtual const std::set<island> getPrivateMemoryIslands(size_t core_id) const;
			virtual const std::set<island>& getVoltageIslands() const;
			virtual const island& getVoltageIsland(size_t core_id) const;
			virtual const std::set<island>& getFrequencyIslands() const;
			virtual const island& getFrequencyIsland(size_t core_id) const;

			virtual const Core* getCore(size_t id) const;
			virtual size_t getCoreId(const Core*) const;
			virtual Algebra buildAlgebra() const;
			virtual	~Platform();
		protected:
			std::set<const Core*> cores;
			std::set<island> shared, main, priv, voltage, freq;
		private:		
	};
}

#endif
