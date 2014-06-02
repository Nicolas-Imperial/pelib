#include <string>

#ifndef PELIB_DATA
#define PELIB_DATA

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
