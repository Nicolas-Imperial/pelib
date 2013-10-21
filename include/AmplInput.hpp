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
#include <AmplInputData.hpp>

#ifndef PELIB_AMPLINPUT
#define PELIB_AMPLINPUT

namespace pelib
{
	class AmplInput: public RecordParser, public RecordOutput
	{
		public:
			AmplInput();

			virtual
			~AmplInput();
			
			virtual
			Record
			parse(std::istream &ampl_data, bool strict = 0);

			virtual
			std::ostream&
			dump(std::ostream& o, const Record &record) const;

			virtual
			AmplInput&
			operator=(const AmplInput &rhs);

		protected:
			//std::vector<AmplDataParser*> parsers;
			//std::vector<AmplDataParser*> outputs;
			std::vector<AmplInputData*> parsers;
			std::vector<AmplInputData*> outputs;

			void
			deleteParsers();

			void
			deleteOutputs();
			
		private:
	};
}

#endif