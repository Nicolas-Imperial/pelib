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
			DataParser(bool strict);
			
			template <class Target>
			static
			Target
			convert(std::string element, bool strict = 0)
			{
				Target out;
				std::istringstream converter(element);
				converter >> out;

				if(converter.fail())
				{
					throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
				}
				
				//std::cerr << element << std::endl;
				//std::cerr << strict << std::endl;

				//std::cerr << "Let us proceed with optional checking" << std::endl;
				
				if(strict)
				{
					//std::cerr << "OK so we have to check" << std::endl;
					// We asked a floating-point conversion, but provided a integer
					if(strcmp(typeid(out).name(), "f") == 0 || strcmp(typeid(out).name(), "d") == 0)
					{
						//std::cerr << "So we are asking for a decimal conversion" << std::endl;
						try
						{
							// let's try to parse against a fixed-point value
							match("\\d+.\\d+", element);
							//std::cerr << element << "Passed the fixed-point format matching" << std::endl;
						} catch(ParseException &e)
						{
							// OK so it doesn't parse a fixed-point notation; let's try scientific notation
							// Then I suppose it was a scientific notation; let's see if it indeed denotes a decimal digit
							long long int int_test;
							std::istringstream converter(element);
							converter >> int_test;

							if(int_test == out)
							{
								// Integer-converted and floating-point were equal, then it was an integer, you fool
								throw ParseException(std::string("Asked a decimal conversion, but \"").append(element).append("\" is integer."));
							}
							//std::cerr << "Passed the scientific format matching" << std::endl;
						}
						//std::cerr << "OK I valid this conversion" << std::endl;
					}
						
					// We asked an integer conversion, but provided a float
					if(strcmp(typeid(out).name(), "f") != 0 && strcmp(typeid(out).name(), "d") != 0)
					{
						//std::cerr << "So we are asking for a integer conversion" << std::endl;
						try
						{
							// let's try to parse against a fixed-point value
							match("\\d+.\\d+", element);
						} catch(ParseException &e)
						{
							//std::cerr << "It is not fixed-point format, good" << std::endl;
							// Then I suppose it was a scientific notation; let's see if it indeed denotes an integer digit
							double int_test;
							std::istringstream converter(element);
							converter >> int_test;

							if(int_test != out)
							{
								//std::cerr << "But this is not an integer anyway" << std::endl;
								// Integer-converted and floating-point were equal, then it was an integer, you fool
								throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));
							}

							//std::cerr << "OK I valid this conversion" << std::endl;
							return out;
						}
						//std::cerr << "But element was fixed-point format" << std::endl;
						throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));

						/*
						int ref = convert<int>(element);
						if(out == ref) // We asked for a floating point unit where an integer could have done it
						{
							throw ParseException(std::string("Requested a floating point container, but a integer container would fit \"").append(element).append("\"."));
						}
					*/

					// OK now I know this element matches a decimal value
					
						// It was not asked to parse agasint any decimal-capable type (float or double)
						// Throw an exception
					}
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
			parse(std::istream &in) = 0;

			virtual
			std::string
			getPattern() = 0;

		protected:
			bool strict;
		private:
	};
}

#endif
