#include <set>

#include <StreamingAppData.hpp>
#include <Algebra.hpp>

#ifndef PELIB_PLATFORM
#define PELIB_PLATFORM

namespace pelib
{
	class Platform : public StreamingAppData
	{
		public:
			Platform();
			Platform(const Platform *arch);
			virtual Platform* clone() const;

			virtual int getCoreNumber() const;
			virtual void setCoreNumber(int p);

			virtual const std::set<float>& getFrequencies() const;
			virtual void setFrequencies(const std::set<float>& freq);

			virtual Algebra buildAlgebra() const;

		protected:
			int coreNumber;
			std::set<float> frequencies;
		private:		
	};
}

#endif
