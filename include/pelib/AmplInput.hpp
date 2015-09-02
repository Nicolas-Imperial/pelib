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
#include <pelib/AmplInputData.hpp>

#ifndef PELIB_AMPLINPUT
#define PELIB_AMPLINPUT

namespace pelib
{
	class AmplInput: public AlgebraParser, public AlgebraOutput
	{
		public:
			//AmplInput(std::vector<AmplInputDataParser*> parsers, std::vector<AmplInputDataOutput*> outputs);
			AmplInput(std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > handlers);
			//AmplInput();

			virtual
			~AmplInput();

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
			AmplInput&
			operator=(const AmplInput &rhs);

			static std::vector<AmplInputDataParser*> floatParsers();
			static std::vector<AmplInputDataOutput*> floatOutputs();
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > floatHandlers();

			static std::vector<AmplInputDataParser*> intFloatParsers();
			static std::vector<AmplInputDataOutput*> intFloatOutputs();
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > intFloatHandlers();

			static std::vector<AmplInputDataParser*> stringParsers();
			static std::vector<AmplInputDataOutput*> stringOutputs();
			static std::pair<std::vector<AmplInputDataParser*>, std::vector<AmplInputDataOutput*> > stringHandlers();

		protected:
			std::vector<AmplInputDataParser*> parsers;
			std::vector<AmplInputDataOutput*> outputs;

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

