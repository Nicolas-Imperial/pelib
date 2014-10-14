#include <set>

#include <StreamingAppData.hpp>
#include <Algebra.hpp>

#ifndef PELIB_ARCHITECTURE
#define PELIB_ARCHITECTURE

namespace pelib
{
	class Architecture : public StreamingAppData
	{
		public:
			Architecture();
			Architecture(const Architecture *arch);
			virtual Architecture* clone() const;

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
