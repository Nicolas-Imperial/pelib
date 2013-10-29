#include <set>

#include <AmplOutputData.hpp>
#include <Set.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>

#ifndef PELIB_AMPLOUTPUTSET
#define PELIB_AMPLOUTPUTSET

namespace pelib
{
	template <class Value>
	class AmplOutputSet: public AmplOutputData
	{
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:			
			virtual
			AmplOutputSet*
			clone() const
			{
				return new AmplOutputSet();
			}

			virtual
			std::string
			getPattern()
			{
				return "set\\s*([^\\s\\n]+)\\s*:=(.*)";
			}

			virtual
			Data*
			parse(std::istream &in, bool strict = 0)
			{
				SetType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match=DataParser::match(getPattern(), str);
				
				boost::regex param_set("\\s*([^\\s]+)");
				std::string remain = match[2];
				const int subs[] = {1};
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_set, subs, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;

				for(; iter != end; ++iter )
				{
					Value value;
					
					value = DataParser::convert<Value>(*iter, strict);

					values.insert(value);
				}

				return new Set<Value>(match[1], values);
			}

			virtual
			void
			dump(std::ostream &o, const Data *data) const
			{				
				const Set<Value> *set = dynamic_cast<const Set<Value>*>(data);
				if(set == NULL) throw CastException("parameter \"data\" was not of type \"Set<Value>\".");
					
				o << "set " << set->getName() << " :=";
				SetType values = set->getValues();
				
				for(typename std::set<Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
				{
					o << " " << *iter;
				}

				o << ";" << std::endl;					
			}

		protected:
		private:	
	};
}

#endif
