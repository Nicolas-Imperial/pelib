#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>
#include <boost/algorithm/string.hpp>

#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>
#include <pelib/Algebra.hpp>
#include <pelib/AlgebraData.hpp>
#include <pelib/DataParser.hpp>

#ifndef PELIB_ALGEBRADATAPARSER
#define PELIB_ALGEBRADATAPARSER

namespace pelib
{
	class AlgebraDataParser : public DataParser
	{
		public:
			// TODO: Make it convert to any class from string
			template <class Target>
			static
			Target
			convert(std::string element, bool strict = 0)
			{
				Target out;
				bool infinity, positive;
				boost::algorithm::to_lower(element);
				
				if(element.compare("+inf") == 0 || element.compare("inf") == 0)
				{
					out = std::numeric_limits<Target>::max();
					infinity = true;
					positive = true;
				}
				else
				{
					if(element.compare("-inf") == 0)
					{
						out = std::numeric_limits<Target>::min();
						infinity = true;
						positive = false;
					}
					else
					{
						std::istringstream converter(element);
						converter >> out;
						infinity = false;

						if(converter.fail())
						{
							throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
						}

						positive = out >= 0;
					}
				}

#if TRACE
				std::cerr << "String element: \"" << element << "\"" << std::endl;
				std::cerr << "Strict checking: \"" << strict << "\"" << std::endl;
				std::cerr << "Let us proceed with optional checking" << std::endl;
#endif
				
				if(strict)
				{
#if TRACE
					std::cerr << "OK so we have to check" << std::endl;
#endif
					// We asked a floating-point conversion, but provided a integer
					if(strcmp(typeid(out).name(), "f") == 0 || strcmp(typeid(out).name(), "d") == 0)
					{
#if TRACE
						std::cerr << "So we are asking for a decimal conversion" << std::endl;
#endif
						try
						{
							// let's try to parse against a fixed-point value
							match("\\d+\\.\\d+", element);
#if TRACE
							std::cerr << "\"" << element << "\" passed the fixed-point format matching" << std::endl;
#endif
						} catch(ParseException &e)
						{
							// OK so it doesn't parse a fixed-point notation
							// Then I suppose it was a scientific notation; let's see if it indeed denotes a decimal digit
							long long int int_test = (long long int)floor(out);
							//std::istringstream converter(element);
							//converter >> int_test;

#if TRACE
							std::cerr << "\"" << element << "\" is not at fixed-point format, may be scientific notation" << std::endl;
#endif

							if(int_test == out && !infinity)
							{
								// Integer-converted and floating-point were equal, then it was an integer, you fool
#if TRACE
								std::cerr << "The decimal part of \"" << element << "\" is nul, therefore we have an integer" << std::endl;
#endif
								throw NoDecimalFloatException(std::string("Asked a decimal conversion, but \"").append(element).append("\" is integer."), out);
							}

							if(infinity && std::numeric_limits<Target>::has_infinity)
							{
								out = positive ? std::numeric_limits<Target>::infinity() : -1 * std::numeric_limits<Target>::infinity();
							}
#if TRACE
							std::cerr << "Passed the scientific format matching" << std::endl;
#endif
						}
#if TRACE
						std::cerr << "OK I valid this conversion" << std::endl;
#endif
					}
						
					// We asked an integer conversion, but provided a float
					if(strcmp(typeid(out).name(), "f") != 0 && strcmp(typeid(out).name(), "d") != 0)
					{
#if TRACE
						std::cerr << "So we are asking for a integer conversion" << std::endl;
#endif
						try
						{
							// let's try to parse against a fixed-point value
							match("\\d+\\.\\d+", element);
						} catch(ParseException &e)
						{
#if TRACE
							std::cerr << "It is not fixed-point format, good" << std::endl;
#endif
							// Then I suppose it was a scientific notation; let's see if it indeed denotes an integer digit
							if(!infinity)
							{
								double int_test;
								std::istringstream converter(element);
								converter >> int_test;

								if(int_test != out)
								{
#if TRACE
									std::cerr << "But this is not an integer anyway" << std::endl;
#endif
									// Integer-converted and floating-point were equal, then it was an integer, you fool
									throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));
								}
#if TRACE
								std::cerr << "OK I valid this conversion" << std::endl;
#endif
								return out;
							}
							else
							{
								if(infinity && std::numeric_limits<Target>::has_infinity)
								{
									return positive ? std::numeric_limits<Target>::infinity() : -1 * std::numeric_limits<Target>::infinity();
								}
								else
								{
									return out;
								}
							}
						}
#if TRACE
						std::cerr << "But element was fixed-point format" << std::endl;
#endif
						throw ParseException(std::string("Asked an integer conversion, but \"").append(element).append("\" is decimal."));
					}
				}
				else
				{
					if(infinity && std::numeric_limits<Target>::has_infinity)
					{
						return positive ? std::numeric_limits<Target>::infinity() : -1 * std::numeric_limits<Target>::infinity();
					}
						else
					{
						return out;
					}
				}

				return out;
			}

			static
			boost::cmatch
			match(std::string regex, std::string str);

			virtual
			std::string
			getDetailedPattern() = 0;

			virtual
			std::string
			getGlobalPattern() = 0;

			virtual
			AlgebraData*
			parse(std::istream &in) = 0;
		protected:
		private:
	};
}

#endif