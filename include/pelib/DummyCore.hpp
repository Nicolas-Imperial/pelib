#include <pelib/Core.hpp>

#ifndef PELIB_DUMMY_CORE
#define PELIB_DUMMY_CORE

namespace pelib
{
	class DummyCore: public Core
	{
		public:
			DummyCore(const std::set<float> &f);
			DummyCore(const DummyCore*);
			DummyCore(const DummyCore&);
			virtual ~DummyCore();
			virtual Core* clone() const;
			virtual const std::set<float>& getFrequencies() const;
		protected:
			std::set<float> frequencies;
		private:
	};
}

#endif
