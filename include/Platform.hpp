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
			Platform(const std::set<const Core*>&);
			Platform(const Platform *arch);
			Platform(const Algebra &arch);
			virtual Platform* clone() const;
			virtual const std::set<const Core*>& getCores() const;
			virtual bool isHomogeneous() const;

			virtual Algebra buildAlgebra() const;
			virtual	~Platform();
		protected:
			std::set<const Core*> cores;
		private:		
	};
}

#endif
