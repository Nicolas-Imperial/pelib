#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplRecordParser.hpp>

#ifndef PARAMSCALAR
#define PARAMSCALAR

namespace pelib
{
	template <class Value>
	class
	ParamScalar: public AmplRecordParser
	{
		protected:
		Value value;

		public:
		ParamScalar(){}
		ParamScalar(const ParamScalar<Value> *scalar) : value(scalar->value) {}

		void
		parse(std::istream &in)
		{	
			std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			boost::cmatch match = AmplRecordParser::match("^param\\s+([^\\s\n]*?)\\s*:=\\s*([^\\s\n]*?)\\s*$", str);

			this->name = match[1];
			this->value = AmplRecordParser::convert<Value>(match[2]);
		}

		Value
		getValue() const
		{
			return this->value;
		}

		virtual
		ParamScalar*
		clone() const
		{
			return new ParamScalar(this);
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const
		{
			return o << "param " << this->name << " := " << this->value << ";" << std::endl;
		}
	};
}

#endif