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
