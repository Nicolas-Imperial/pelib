#include <StreamingAppData.hpp>

#ifndef PELIB_ARCHITECTURE
#define PELIB_ARCHITECTURE

namespace pelib
{
	class Architecture : public StreamingAppData
	{
		public:
			Architecture(const std::string name);
			Architecture(const Architecture *arch);
			virtual Architecture* clone() const;

		protected:
		private:		
	};
}

#endif
