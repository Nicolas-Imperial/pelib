#include <pelib/AmplOutputData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_AMPLOUTPUTVECTOR
#define PELIB_AMPLOUTPUTVECTOR

namespace pelib
{
	template <class Key, class Value>
	class AmplOutputVector: public AmplOutputData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			AmplOutputVector(bool strict = true)
			{
				this->strict = strict;
			}
			virtual
			AmplOutputVector*
			clone() const
			{
				return new AmplOutputVector();
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "(\\w[\\w\\d_]*)\\s*\\[\\*\\]\\s*:=\\s*((?:[\\w\\d][\\w\\d_+\\.]*\\s+[\\w\\d][\\w\\d_\\.+]*[\\s\\n]+)+)";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "\\w[\\w\\d_]*\\s*\\[\\*\\]\\s*:=.+";
			}

			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				//std::cerr << str << std::endl;
				//std::cerr << std::string("(?:.*?)").append(getDetailedPattern()) << std::endl;
				boost::cmatch match = AlgebraDataParser::match(std::string("(?:.*?)").append(getDetailedPattern()), str);
				
				boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
				std::string remain = match[2];
				const int subs[] = {1, 2};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				int integer_values = 0, total_values = 0;
				for(; iter != end; ++iter )
				{
					Key key;
					Value value;

					std::string key_str = *iter++;
					std::string val_str = *iter;

					try
					{
						key = AlgebraDataParser::convert<Key>(key_str, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
					
					try
					{
						value = AlgebraDataParser::convert<Value>(val_str, strict);
					} catch(NoDecimalFloatException &e)
					{
						float float_value = e.getValue();
						value = (Value)float_value;
						integer_values++;
					}

					values.insert(std::pair<Key, Value>(key, value));
					total_values++;
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Vector only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Vector only composed of integer-parsable values."));
				}

				return new Vector<Key, Value>(match[1], values);
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
				
				o << vector->getName() << " [*] := " << std::endl;

				// warning: "iter++" makes valgrind unhappy, dealing with free'd memory
				for(typename std::map<Key, Value>::const_iterator iter = vector->getValues().begin(); iter != vector->getValues().end(); iter++)
				{
					o << iter->first << "\t" << iter->second << std::endl;
				}
				
				o << ";" << std::endl;		
			}

		protected:
			bool strict;
		private:	
	};
}

#endif
