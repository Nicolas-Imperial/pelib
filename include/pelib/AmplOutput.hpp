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


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/AlgebraParser.hpp>
#include <pelib/AlgebraOutput.hpp>
#include <pelib/AmplDataParser.hpp>
#include <pelib/AmplDataOutput.hpp>
#include <pelib/AmplOutputData.hpp>

#ifndef PELIB_AMPLOUTPUT
#define PELIB_AMPLOUTPUT

namespace pelib
{
	class AmplOutput: public AlgebraParser, public AlgebraOutput
	{
		public:
			//AmplOutput(std::vector<AmplOutputDataParser*> parsers, std::vector<AmplOutputDataOutput*> outputs);
			AmplOutput(std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > handlers);
			//AmplOutput();

			virtual
			~AmplOutput();
			
			Algebra
			parse(std::istream &data) const;

			virtual
			void
			dump(std::ostream& o, const Algebra &record) const;

			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const;

			virtual
			void
			dump(std::ostream& o, const AlgebraData &data) const;

			virtual
			AmplOutput&
			operator=(const AmplOutput &rhs);

			static std::vector<AmplOutputDataParser*> floatParsers();
			static std::vector<AmplOutputDataOutput*> floatOutputs();
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > floatHandlers();

			static std::vector<AmplOutputDataParser*> intFloatParsers();
			static std::vector<AmplOutputDataOutput*> intFloatOutputs();
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > intFloatHandlers();

			static std::vector<AmplOutputDataParser*> stringParsers();
			static std::vector<AmplOutputDataOutput*> stringOutputs();
			static std::pair<std::vector<AmplOutputDataParser*>, std::vector<AmplOutputDataOutput*> > stringHandlers();
		protected:
			std::vector<AmplOutputDataParser*> parsers;
			std::vector<AmplOutputDataOutput*> outputs;

			void
			deleteParsers();

			void
			deleteOutputs();
/*
			void
			addParsers();

			void
			addOutputs();
	*/		
		private:
	};
}

#endif
