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

#include <map>
#include <pelib/AlgebraSolver.hpp>

#ifndef PELIB_AMPLSOLVER
#define PELIB_AMPLSOLVER
namespace pelib
{
	class AmplSolver : public AlgebraSolver
	{
		public:
			AmplSolver(std::istream &model, const std::string &model_filename, std::istream &run, bool showOutput = false, bool showError = false);
			AmplSolver(std::istream &model, const std::string &model_filename, std::istream &run, const std::map<std::string, const Algebra> &data, bool showOutput = false, bool showError = false);
			pelib::Algebra* solve() const;
			pelib::Algebra* solve(const std::map<std::string, const Algebra> &data) const;
		protected:
			std::string model, model_file, run;
			std::map<std::string, const Algebra> data;
			bool showOutput, showError;
		private:
			void initModelFromStream(std::istream&, const std::string&, std::istream&, bool showOutput = false, bool showError = false);
	};
}

#endif
