#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AlgebraParser.hpp>
#include <AlgebraOutput.hpp>
#include <AmplDataParser.hpp>
#include <AmplDataOutput.hpp>
#include <AmplOutputData.hpp>

#ifndef PELIB_AMPLOUTPUT
#define PELIB_AMPLOUTPUT

namespace pelib
{
	class AmplOutput: public AlgebraParser, public AlgebraOutput
	{
		public:
			AmplOutput(std::vector<AmplOutputDataParser*> parsers,
			std::vector<AmplOutputDataOutput*> outputs);
			AmplOutput();

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
			AmplOutput&
			operator=(const AmplOutput &rhs);

		protected:
			std::vector<AmplOutputDataParser*> parsers;
			std::vector<AmplOutputDataOutput*> outputs;

			void
			deleteParsers();

			void
			deleteOutputs();

			void
			addParsers();

			void
			addOutputs();
			
		private:
	};
}

#endif
