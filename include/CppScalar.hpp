#include <CppData.hpp>
#include <DataParser.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>
#include <ParseException.hpp>
#include <NoDecimalFloatException.hpp>

#ifndef PELIB_CPPSCALAR
#define PELIB_CPPSCALAR

namespace pelib
{
	template <class Value>
	class
	CppScalar: public CppData
	{
		public:
			CppScalar() : CppData()
			{
				// Do nothing
			}
			
			virtual
			CppScalar*
			clone() const
			{
				return new CppScalar();
			}

			virtual
			void
			dump(std::ostream &stream, const Data *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				stream << "pelib::Scalar<" << type_name(typeid(scalar->getValue())).c_str() << "> " << scalar->getName() << "(\"" << scalar->getName() << "\", " << scalar->getValue() << ");" << std::endl;
			}
	
		protected:
		private:		
	};
}

#endif
