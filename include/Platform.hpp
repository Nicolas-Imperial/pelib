#include <set>

#include <Algebra.hpp>

#ifndef PELIB_PLATFORM
#define PELIB_PLATFORM

namespace pelib
{
	class Platform : public Record
	{
		public:
			Platform();
			Platform(const Platform *arch);
			Platform(const Algebra &arch);
			virtual Platform* clone() const;

			virtual int getCoreNumber() const;
			virtual void setCoreNumber(int p);

			virtual const std::set<float>& getFrequencies() const;
			virtual void setFrequencies(const std::set<float>& freq);

			virtual Algebra buildAlgebra() const;
			virtual	~Platform();

		protected:
			int coreNumber;
			std::set<float> frequencies;
		private:		
	};
}

#endif
