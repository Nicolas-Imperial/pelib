#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <RecordParser.hpp>
#include <RecordOutput.hpp>
#include <AmplDataParser.hpp>
#include <AmplDataOutput.hpp>
#include <AmplOutputData.hpp>

#ifndef PELIB_AMPLOUTPUT
#define PELIB_AMPLOUTPUT

namespace pelib
{
	class AmplOutput: public RecordParser, public RecordOutput
	{
		public:
			AmplOutput(std::vector<AmplOutputData*> parsers,
			std::vector<AmplOutputData*> outputs);
			AmplOutput();

			virtual
			~AmplOutput();
			
			virtual 	
			Record
			parse(std::istream &ampl_data, bool strict = 0);

			virtual
			void
			dump(std::ostream& o, const Record &record) const;

			virtual
			void
			dump(std::ostream& o, const Data *data) const;

			virtual
			AmplOutput&
			operator=(const AmplOutput &rhs);

		protected:
			//std::vector<AmplDataParser*> parsers;
			//std::vector<AmplDataParser*> outputs;
			std::vector<AmplOutputData*> parsers;
			std::vector<AmplOutputData*> outputs;

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