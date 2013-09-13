#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <ParseException.hpp>
#include <AmplRecord.hpp>

#ifndef AMPLRECORDPARSER
#define AMPLRECORDPARSER

namespace pelib
{
	class AmplRecordParser: public AmplRecord
	{
		public:
		static
		boost::cmatch
		match(std::string regex, std::string str);

		template <class Target>
		static
		Target
		convert(std::string element)
		{
			Target out;
			std::istringstream converter(element);
			converter >> out;

			if(converter.fail())
			{
				throw ParseException("Couln't convert literal into desired type");
			}

			return out;
		}

		virtual
		AmplRecordParser*
		clone() const = 0;

		virtual
		void
		parse(std::istream &in) = 0;

		
	};
}

#endif

