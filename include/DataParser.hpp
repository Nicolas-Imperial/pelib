#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <ParseException.hpp>
#include <Record.hpp>
#include <Data.hpp>

#ifndef PELIB_DATAPARSER
#define PELIB_DATAPARSER

namespace pelib
{
	class DataParser
	{
		public:
			template <class Target>
			static
			Target
			convert(std::string element, bool strict = 0)
			{
				Target out;
				std::istringstream converter(element);
				converter >> out;

				if(strict)
				{
					if(strcmp(typeid(out).name(), "f") == 0 || strcmp(typeid(out).name(), "d") == 0) // We want to parse a floating-point value
					{
						int ref = convert<int>(element);
						if(out == ref) // We asked for a floating point unit where an integer could have done it
						{
							throw ParseException(std::string("Requested a floating point container, but a integer container would fit \"").append(element).append("\"."));
						}
					}
				}

				if(converter.fail())
				{
					throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
				}

				return out;
			}

			static
			boost::cmatch
			match(std::string regex, std::string str);

			virtual
			DataParser*
			clone() const = 0;
		
			virtual
			Data*
			parse(std::istream &in, bool strict = 0) = 0;

			virtual
			std::string
			getPattern() = 0;

		protected:
		private:
	};
}

#endif
