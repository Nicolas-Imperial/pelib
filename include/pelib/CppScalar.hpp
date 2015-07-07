#include <pelib/CppData.hpp>
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

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
			dump(std::ostream &stream, const AlgebraData *data) const
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
