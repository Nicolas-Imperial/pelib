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
#include <pelib/CppDataOutput.hpp>
#include <pelib/CppData.hpp>

#ifndef PELIB_CPP
#define PELIB_CPP

namespace pelib
{
	class Cpp: public AlgebraOutput
	{
		public:
			Cpp(std::vector<CppData*> outputs);
			Cpp(const Cpp &src);
			Cpp();
			Cpp(const std::string& name);

			virtual
			~Cpp();

			virtual
			void
			setName(const std::string &name);

			virtual
			std::string
			getName() const;
			
			virtual
			void
			dump(std::ostream& o, const Algebra &record) const;

			virtual
			void
			dump(std::ostream& o, const AlgebraData *data) const;

			virtual
			Cpp&
			operator=(const Cpp &rhs);

		protected:
			//std::vector<CppParser*> outputs;
			std::vector<CppData*> outputs;
			std::string name;

			void
			deleteOutputs();

			void
			addOutputs();
			
		private:
	};
}

#endif
