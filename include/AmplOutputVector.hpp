#include <AmplOutputData.hpp>
#include <Vector.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>

#ifndef PELIB_AMPLOUTPUTVECTOR
#define PELIB_AMPLOUTPUTVECTOR

namespace pelib
{
	template <class Key, class Value>
	class AmplOutputVector: public AmplOutputData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			AmplOutputVector(bool strict = false) : AmplOutputData(strict)
			{
				// Do nothing
			}
			virtual
			AmplOutputVector*
			clone() const
			{
				return new AmplOutputVector();
			}

			virtual
			std::string
			getPattern()
			{
				return "(\\w+?[\\w\\d_]*?)\\s*\\[\\*\\]\\s*:=\\s*((?:[\\w\\d]+[\\w\\d_]*\\s+[\\w\\d]+[\\w\\d_\\.]*[\\s\\n]+)*)";
			}

			virtual
			Data*
			parse(std::istream &in)
			{
				VectorType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match = DataParser::match(std::string("(?:.*?)").append(getPattern()), str);
				
				boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
				std::string remain = match[2];
				const int subs[] = {1, 2};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				for(; iter != end; ++iter )
				{
					Key key;
					Value value;

					std::string key_str = *iter++;
					std::string val_str = *iter;

					key = DataParser::convert<Value>(key_str, strict);
					value = DataParser::convert<Value>(val_str, strict);

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
				
				o << vector->getName() << " [*] := " << std::endl;

				// warning: "iter++" makes valgrind unhappy, dealing with free'd memory
				for(typename std::map<Key, Value>::const_iterator iter = vector->getValues().begin(); iter != vector->getValues().end(); iter++)
				{
					o << iter->first << "\t" << iter->second << std::endl;
				}
				
				o << ";" << std::endl;		
			}

		protected:
		private:	
	};
}

#endif
