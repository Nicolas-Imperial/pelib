#include <pelib/AmplInputData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/CastException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLINPUTVECTOR
#define PELIB_AMPLINPUTVECTOR

namespace pelib
{
	template <class Key, class Value>
	class AmplInputVector: public AmplInputData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			AmplInputVector(bool strict = true)
			{
				this->strict = strict;
			}
			
			virtual
			AmplInputVector*
			clone() const
			{
				return new AmplInputVector();
			}
			
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match = AlgebraDataParser::match(getDetailedPattern(), str);
				
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

					try
					{
						key = AlgebraDataParser::convert<Key>(*iter++, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
					
					try
					{
						value = AlgebraDataParser::convert<Value>(*iter, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::stringstream ss;
						ss << e.getValue();
						ss >> value;
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
					
				o << "param: " << vector->getName() << " :=" << std::endl;
				VectorType values = vector->getValues();
				
				for(typename std::map<Key, Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
				{
					o << iter->first << " " << iter->second << std::endl;
				}
			
				o << ";" << std::endl;				
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s*:\\s*([^\\s\\n]+)\\s*:=(.+)";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s*:\\s*[^\\s\\n]+\\s*:=.+";
			}

		protected:
			bool strict;
		private:	
	};
}

#endif
