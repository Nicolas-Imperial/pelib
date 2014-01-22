#include <string>

#ifndef PELIB_DATA
#define PELIB_DATA

namespace pelib
{
	class Data
	{
		public:
			Data(const std::string);
			Data(const Data*);
			virtual
			~Data() = 0;
			
			virtual
			const std::string&
			getName() const;

			virtual
			void
			setName(const std::string);

			virtual
			Data*
			clone() const = 0;
			
		protected:
			std::string name;
		private:		
	};
}

#endif
