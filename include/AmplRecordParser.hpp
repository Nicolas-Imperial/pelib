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

			/*
			if(typeid(out).compare("f") == 0 || typeid(out).compare("d") == 0) // We want to parse a floating-point value
			{
				int ref = convert<int>(element);
				if(out == ref) // We asked for a floating point unit where an integer could have done it
				{
					throw ParseException("Requested a floating point container, but a integer container would fit");
				}
			}
			*/

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

