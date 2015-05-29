#include <string>

#ifndef PELIB_DATA
#define PELIB_DATA

#if DEBUG
#define trace(var) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"." << endl
#else
#define trace(var)
#endif

namespace pelib
{
	class Data
	{
		public:
			virtual ~Data();

			virtual
			Data*
			clone() const = 0;
			
		protected:
		private:		
	};
}

#endif
