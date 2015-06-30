#include <set>

#ifndef PELIB_CORE
#define PELIB_CORE

namespace pelib
{
	class Core
	{
		public:
			virtual Core* clone() const = 0;
			virtual const std::set<float>& getFrequencies() const = 0;
		protected:
		private:
	};
}

#endif
