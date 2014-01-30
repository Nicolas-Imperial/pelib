#include <AmplInputData.hpp>
#include <DataParser.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>
#include <ParseException.hpp>
#include <NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLINPUTSCALAR
#define PELIB_AMPLINPUTSCALAR

namespace pelib
{
	template <class Value>
	class
	AmplInputScalar: public AmplInputData
	{
		public:
			AmplInputScalar(bool strict = true) : AmplInputData(strict)
			{
				// Do nothing
			}
			
			virtual
			AmplInputScalar*
			clone() const
			{
				return new AmplInputScalar();
			}
			
			virtual
			Data*
			parse(std::istream &in)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

				boost::cmatch match;
				try
				{
					match = DataParser::match(getDetailedPattern(), str);
				} catch(NoDecimalFloatException &e)
				{
					std::ostringstream ss;
					ss << e.getValue();
					throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));
				}

				std::string match1 = match[1];
				std::string match2 = match[2];
				Value val = DataParser::convert<Value>(match2, strict);
				Scalar<Value> *scalar = new Scalar<Value>(match1, val);
				return scalar;
			}

			virtual
			void
			dump(std::ostream &stream, const Data *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				stream << "param " << scalar->getName() << " := " << scalar->getValue() << ";" << std::endl;
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s+([^\\s\\n]*)\\s*:=\\s*([^\\s]+)\\s*";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s+[^\\s\\n]*\\s*:=.+";
			}
	
		protected:
		private:		
	};
}

#endif
