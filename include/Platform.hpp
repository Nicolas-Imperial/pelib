#include <set>

#include <Algebra.hpp>

#ifndef PELIB_PLATFORM
#define PELIB_PLATFORM

namespace pelib
{
	class Platform : public Record
	{
		public:
			Platform(size_t p, const std::set<float> &f);
			Platform(const Platform *arch);
			Platform(const Algebra &arch);
			virtual Platform* clone() const;

			virtual int getCoreNumber() const;

			virtual const std::set<float>& getFrequencies() const;

			virtual Algebra buildAlgebra() const;
			virtual	~Platform();

		protected:
			size_t coreNumber;
			std::set<float> frequencies;
		private:		
	};
}

#endif
