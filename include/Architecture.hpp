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

			virtual const std::set<int, std::less<int>, std::allocator<int> >& getFrequencies() const;
			virtual void setFrequencies(const std::set<int, std::less<int>, std::allocator<int> >& freq);

			virtual Algebra buildAlgebra() const;

		protected:
			int coreNumber;
			std::set<int, std::less<int>, std::allocator<int> > frequencies;
		private:		
	};
}

#endif
