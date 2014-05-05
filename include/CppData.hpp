#include <CppDataOutput.hpp>

#ifndef PELIB_CPPDATA
#define PELIB_CPPDATA

namespace pelib
{
	class CppData: public CppDataOutput
	{
		public:
			CppData(bool strict) {}
                        CppData(){}
			virtual
			CppData*
			clone() const = 0;
	  virtual ~CppData(){}
		protected:
			const std::string
			type_name(const std::type_info &type) const;
		private:		
	};
}

#endif
