/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <set>

#include <pelib/AmplOutputData.hpp>
#include <pelib/Set.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_AMPLOUTPUTSET
#define PELIB_AMPLOUTPUTSET

namespace pelib
{
	/*** AMPL Output format parser and output class for pelib::Set class instances ***/
	template <class Value>
	class AmplOutputSet: public AmplOutputData
	{
		/*** Internal format of data collection handled ***/
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			/** Constructor
				@param strict If elements in Set are floating-point numeric values and strict is true, then parser fails if any element parsed is written as an integer. A trailing .0 (e.g. 52.0) can force an integer to be parsed as floating-point without failure.
			**/
			AmplOutputSet(bool strict = true)
			{
				this->strict = strict;
			}
			
			/** Returns a pointer to a copy of this class instance **/
			virtual
			AmplOutputSet*
			clone() const
			{
				return new AmplOutputSet();
			}

			/** Returns a boost::regex regular expression able to match a AMPL output format Set and extract its elements **/
			virtual
			std::string
			getDetailedPattern()
			{
				return "set\\s+([^\\s\\n]+)\\s*:=(.+)";
			}

			/** Returns a boost::regex regular expression able to match a AMPL output format Set **/
			virtual
			std::string
			getGlobalPattern()
			{
				return "set\\s+[^\\s\\n]+\\s*:=.+";
			}

			/** Parses input stream in AMPL output format and builds an instance of pelib::Set from the data extracted
				@param in input stream to read data from
			**/
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				SetType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
				boost::cmatch match=AlgebraDataParser::match(getDetailedPattern(), str);
				//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
				
				boost::regex param_set("\\s*([^\\s]+)");
				std::string remain = match[2];
				const int subs[] = {1};
				//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
				boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_set, subs, boost::regex_constants::match_default);
				//std::cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
				boost::sregex_token_iterator end;

				int integer_values = 0, total_values = 0;
				for(; iter != end; ++iter )
				{
					Value value;
					
					try
					{
						value = AlgebraDataParser::convert<Value>(*iter, strict);
					} catch(NoDecimalFloatException &e)
					{
						float float_value = e.getValue();
						std::stringstream ss;
						ss << float_value;
						ss >> value;
						integer_values++;
					}

					values.insert(value);
					total_values++;
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Set only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Set only composed of integer-parsable values."));
				}

				return new Set<Value>(match[1], values);
			}

			/** Write the content of an instance of pelib::Set into output stream in AMPL output format
				@param o Output stream where the set is written to in AMPL output format
				@param data Instance of pelib::Set to write to output stream
			**/
			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
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
			/** Defines if parsing operation happens in strict mode or not **/
			bool strict;
		private:	
	};
}

#endif
