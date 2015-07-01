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
			Platform();
			Platform(size_t p, const Core* ref);
			Platform(const std::set<const Core*>&);
			Platform(const std::set<const Core*>&, const std::set<std::set <const Core*> >& shared, const std::set<std::set <const Core*> >& main, const std::set<std::set <const Core*> >& priv, const std::set<std::set <const Core*> >& voltage, const std::set<std::set <const Core*> >& freq);
			Platform(const Platform *arch);
			Platform(const Algebra &arch);
			virtual Platform* clone() const;
			virtual const std::set<const Core*>& getCores() const;
			virtual bool isHomogeneous() const;
			virtual const std::set<std::set<const Core*> >& getSharedMemoryIslands() const;
			virtual const std::set<const Core*>& getSharedMemoryIslands(size_t id) const;
			virtual const std::set<std::set<const Core*> >& getMainMemoryIslands() const;
			virtual const std::set<const Core*>& getMainMemoryIslands(size_t id) const;
			virtual const std::set<std::set<const Core*> >& getPrivateMemoryIslands() const;
			virtual const std::set<const Core*>& getPrivateMemoryIslands(size_t id) const;
			virtual const std::set<std::set<const Core*> >& getVoltageIslands() const;
			virtual const std::set<const Core*>& getVoltageIslands(size_t id) const;
			virtual const std::set<std::set<const Core*> >& getFrequencyIslands() const;
			virtual const std::set<const Core*>& getFrequencyIslands(size_t id) const;
			virtual const Core* getCore(size_t id) const;
			virtual size_t getCoreId(const Core*) const;
			virtual Algebra buildAlgebra() const;
			virtual	~Platform();
		protected:
			std::set<const Core*> cores;
			std::set<std::set <const Core*> > shared, main, priv, voltage, freq;
		private:		
	};
}

#endif
