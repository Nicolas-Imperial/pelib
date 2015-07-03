#include <set>

#include <Core.hpp>
#include <Algebra.hpp>

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
			virtual const std::set<island> getSharedmemoryIslands(const std::set<int>& islands) const;
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
