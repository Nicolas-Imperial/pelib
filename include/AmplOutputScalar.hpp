#include <AmplOutputData.hpp>
#include <DataParser.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>
#include <ParseException.hpp>
#include <NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLOUTPUTSCALAR
#define PELIB_AMPLOUTPUTSCALAR

namespace pelib
{
	template <class Value>
	class
	AmplOutputScalar: public AmplOutputData
	{
		public:
			AmplOutputScalar(bool strict = true)
			{
				this->strict = strict;
			}
			
			virtual
			AmplOutputScalar*
			clone() const
			{
				return new AmplOutputScalar();
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "(\\w[\\w\\d_]*)\\s*=\\s*([\\w\\d][\\w\\d_\\.]*)";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "\\w[\\w\\d_]*\\s*=\\s*[\\w\\d][\\w\\d_\\.]*";
			}

			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

				boost::cmatch match;
				try
				{
					match = AlgebraDataParser::match(std::string("(?:.*?)").append(getDetailedPattern()), str);
				} catch(NoDecimalFloatException &e)
				{
					std::ostringstream ss;
					ss << e.getValue();
					throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));
				}

				Scalar<Value> *scalar = new Scalar<Value>(match[1], AlgebraDataParser::convert<Value>(match[2], strict));
				return scalar;
			}

			virtual
			void
			dump(std::ostream &stream, const AlgebraData *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				stream << scalar->getName() << " = " << scalar->getValue() << std::endl;
			}
	
		protected:
			bool strict;
		private:		
	};
}

#endif
