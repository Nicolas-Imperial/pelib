#include <AmplOutputData.hpp>
#include <DataParser.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>
#include <ParseException.hpp>

#ifndef PELIB_AMPLOUTPUTSCALAR
#define PELIB_AMPLOUTPUTSCALAR

namespace pelib
{
	template <class Value>
	class
	AmplOutputScalar: public AmplOutputData
	{
		public:			
			virtual
			AmplOutputScalar*
			clone() const
			{
				return new AmplOutputScalar();
			}

			virtual
			std::string
			getPattern()
			{
				return "(\\w+?[\\w\\d_]*?)\\s*=\\s*([\\w\\d]+?[\\w\\d_\\.]*)";
			}

			virtual
			Data*
			parse(std::istream &in, bool strict = 0)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match = DataParser::match(std::string("(?:.*?)").append(getPattern()), str);

				Scalar<Value> *scalar = new Scalar<Value>(match[1], DataParser::convert<Value>(match[2], strict));
				return scalar;
			}

			virtual
			std::ostream&
			dump(std::ostream &stream, const Data *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				return stream << scalar->getName() << " = " << scalar->getValue() << std::endl;
			}
	
		protected:
		private:		
	};
}

#endif
