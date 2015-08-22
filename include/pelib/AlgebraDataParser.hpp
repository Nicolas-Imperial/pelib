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
			template <class Target>
			static
			Target
			convert(std::string element, bool strict = 0)
			{
				Target out;
				bool infinity, positive;
				boost::algorithm::to_lower(element);

				if(is_integer(out) || is_decimal(out))
				{
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

							std::stringstream ss;
							ss << out;
							double val;
							ss >> val;

							positive = val >= 0;
						}
					}
				}
				else
				{
					std::istringstream converter(element);
					converter >> out;

					if(converter.fail())
					{
						throw ParseException(std::string("Couln't convert literal \"").append(element).append("\" into type \"").append(typeid(out).name()).append("\"."));
					}
				
					return out;	
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
					//if(strcmp(typeid(out).name(), "f") == 0 || strcmp(typeid(out).name(), "d") == 0)
					if(is_decimal<Target>(out) && ! is_integer<Target>(out))
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
							double val;
							std::stringstream ss;
							ss << out;
							ss >> val;
							long long int int_test = (long long int)floor(val);
							//std::istringstream converter(element);
							//converter >> int_test;

#if TRACE
							std::cerr << "\"" << element << "\" is not at fixed-point format, may be scientific notation" << std::endl;
#endif

							if(int_test == val && !infinity)
							{
								// Integer-converted and floating-point were equal, then it was an integer, you fool
#if TRACE
								std::cerr << "The decimal part of \"" << element << "\" is nul, therefore we have an integer" << std::endl;
#endif
								throw NoDecimalFloatException(std::string("Asked a decimal conversion, but \"").append(element).append("\" is integer."), val);
							}

							if(infinity && std::numeric_limits<Target>::has_infinity)
							{
								ss.str("");
								ss.clear(); // Clear state flags.
								ss << std::numeric_limits<Target>::infinity();
								ss >> val;

								ss.str("");
								ss.clear();
								ss << (positive ? val : -1 * val);
								ss >> out;
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
					//if(strcmp(typeid(out).name(), "f") != 0 && strcmp(typeid(out).name(), "d") != 0)
					if(! is_decimal<Target>(out) && is_integer<Target>(out))
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

								long long int val;
								std::stringstream ss;
								ss << out;
								ss >> val;

								if(int_test != val)
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
									long long int val;
									std::stringstream ss;
									ss << std::numeric_limits<Target>::infinity();
									ss >> val;
	
									ss.str("");
									ss.clear();
									ss << (positive ? val : -1 * val);
									ss >> out;
	
									return out;
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
						std::stringstream ss;
					
						if(is_decimal(out))
						{
							long double val;
							ss << std::numeric_limits<Target>::infinity();
							ss >> val;
							ss.str("");
							ss.clear();
							ss << (positive ? val : -1 * val);
						}
						else
						{
							long long int val;
							ss << std::numeric_limits<Target>::infinity();
							ss >> val;
							ss.str("");
							ss.clear();
							ss << (positive ? val : -1 * val);
						}

						ss >> out;
						return out;
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
			template <class Target>
			static bool is_decimal(Target var)
			{
				return (std::string(typeid(var).name()).compare(std::string(typeid(float).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(double).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(long double).name())) == 0);
			}

			template <class Target>
			static bool is_integer(Target var)
			{
				/*
					List obtained by running the following C program
					#include <iostream>
					#include <cstdlib>
					#include <typeinfo>

					using namespace std;

					#define show(type) cout << #type << " " << typeid(type).name() << endl;

					int
					main(int argc, char **argv)
					{
						show(int);
						show(unsigned int);
						show(long int);
						show(long unsigned int);
						show(long long int);
						show(long long unsigned int);
						cout << endl;
						show(size_t);
						show(ptrdiff_t);
						show(signed size_t);
						show(unsigned ptrdiff_t);
						show(long size_t);
						show(long long size_t);
						show(long ptrdiff_t);
						show(long long ptrdiff_t);
						cout << endl;
						show(float);
						show(double);
						show(long double);
						return EXIT_SUCCESS;
					}

					./test | rev | sort -k 1 | uniq -w 1 | cut -f 1 -d ' ' --complement | rev
				*/

				return (std::string(typeid(var).name()).compare(std::string(typeid(int).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(unsigned int).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(long size_t).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(size_t).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(long long size_t).name())) == 0) ||
					(std::string(typeid(var).name()).compare(std::string(typeid(long long unsigned int).name())) == 0);
			}			
		private:
			// Nothing private
	};
}

#endif
