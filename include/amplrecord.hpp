#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <parseexception.hpp>

#ifndef AMPLRECORD
#define AMPLRECORD

namespace pelib
{
	class AmplRecord
	{
		protected:
		std::string name;

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

		public:
		virtual std::ostream&
		dump(std::ostream& o) const = 0;

		virtual
		void
		parse(std::istream &in) = 0;

		std::string
		getName() const;

		virtual
		AmplRecord*
		clone() const = 0;
	};

	std::ostream&
	operator<<(std::ostream& o, const AmplRecord& r);
}

#endif

