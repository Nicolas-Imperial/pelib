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
#include <AmplInputData.hpp>

#ifndef PELIB_AMPLINPUT
#define PELIB_AMPLINPUT

namespace pelib
{
	class AmplInput: public AlgebraParser, public AlgebraOutput
	{
		public:
			AmplInput(std::vector<AmplInputDataParser*> parsers,
			std::vector<AmplInputDataOutput*> outputs);
			AmplInput();

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
			AmplInput&
			operator=(const AmplInput &rhs);

		protected:
			std::vector<AmplInputDataParser*> parsers;
			std::vector<AmplInputDataOutput*> outputs;

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

