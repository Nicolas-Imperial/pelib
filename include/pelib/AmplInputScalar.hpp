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


#include <pelib/AmplInputData.hpp>
#include <pelib/DataParser.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/NoDecimalFloatException.hpp>

#ifndef PELIB_AMPLINPUTSCALAR
#define PELIB_AMPLINPUTSCALAR

namespace pelib
{
	template <class Value>
	class
	AmplInputScalar: public AmplInputData
	{
		public:
			AmplInputScalar(bool strict = true)
			{
				this->strict = strict;
			}
			
			virtual
			AmplInputScalar*
			clone() const
			{
				return new AmplInputScalar();
			}
			
			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

				boost::cmatch match;
				try
				{
					match = AlgebraDataParser::match(getDetailedPattern(), str);
				} catch(NoDecimalFloatException &e)
				{
					std::ostringstream ss;
					ss << e.getValue();
					throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));
				}

				std::string match1 = match[1];
				std::string match2 = match[2];
				Value val = AlgebraDataParser::convert<Value>(match2, strict);
				Scalar<Value> *scalar = new Scalar<Value>(match1, val);
				return scalar;
			}

			virtual
			void
			dump(std::ostream &stream, const AlgebraData *data) const
			{
				const Scalar<Value> *scalar = dynamic_cast<const Scalar<Value>* >(data);
				if(scalar == NULL) throw CastException("parameter \"data\" was not of type \"Scalar<Value>\".");

				stream << "param " << scalar->getName() << " := " << scalar->getValue() << ";" << std::endl;
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s+([^\\s\\n]*)\\s*:=\\s*([^\\s]+)\\s*";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s+[^\\s\\n]*\\s*:=.+";
			}
	
		protected:
			bool strict;
		private:		
	};
}

#endif
