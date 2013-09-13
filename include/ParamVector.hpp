#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplRecord.hpp>

namespace pelib
{
	template <class Key, class Value>
	class
	ParamVector: public AmplRecord
	{
		protected:
		std::map<Key, Value> values;
		
		public:
		ParamVector(){}
		ParamVector(const ParamVector<Key, Value> *vector) : values(vector->values) {}

		void
		parse(std::istream &in)
		{
			std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			boost::cmatch match=AmplRecord::match("^param\\s*:\\s*([^\\s\\n]+)\\s*:=(.*)$", str);
			this->name = match[1];

			boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
			std::string remain = match[2];
			const int subs[] = {1, 2};
			boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, boost::regex_constants::match_default);
			boost::sregex_token_iterator end;

			for(; iter != end; ++iter )
			{
				Key key;
				Value value;

				key = AmplRecord::convert<Value>(*iter++);
				value = AmplRecord::convert<Value>(*iter);

				values.insert(std::pair<Key, Value>(key, value));
			}
		}

		virtual
		std::map<Key, Value>
		getValues() const
		{
			return values;
		}

		virtual
		Value
		find(Key key)
		{
			return values.find(key)->second;
		}

		virtual
		size_t
		getSize()
		{
			return values.size();
		}

		virtual
		ParamVector*
		clone() const
		{
			return new ParamVector(this);
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const 
		{
			o << "param: " << this->name << " :=" << std::endl;
			for(typename std::map<Key, Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
			{
				o << iter->first << " " << iter->second << std::endl;
			}
			return o << ";" << std::endl;
		}
	};
}
