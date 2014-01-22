#include <AmplInputData.hpp>
#include <Vector.hpp>
#include <CastException.hpp>

#ifndef PELIB_AMPLINPUTVECTOR
#define PELIB_AMPLINPUTVECTOR

namespace pelib
{
	template <class Key, class Value>
	class AmplInputVector: public AmplInputData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			AmplInputVector(bool strict = false) : AmplInputData(strict)
			{
				// Do nothing
			}
			
			virtual
			AmplInputVector*
			clone() const
			{
				return new AmplInputVector();
			}
			
			virtual
			Data*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match=DataParser::match(getPattern(), str);
				
				boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
				std::string remain = match[2];
				const int subs[] = {1, 2};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				for(; iter != end; ++iter )
				{
					Key key;
					Value value;

					key = DataParser::convert<Value>(*iter++, strict);
					value = DataParser::convert<Value>(*iter, strict);

					values.insert(std::pair<Key, Value>(key, value));
				}

				return new Vector<Key, Value>(match[1], values);
			}

			virtual
			void
			dump(std::ostream &o, const Data *data) const
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
			getPattern()
			{
				return "param\\s*:\\s*([^\\s\\n]+)\\s*:=(.*)";
			}

		protected:
		private:	
	};
}

#endif
