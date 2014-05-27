#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AlgebraParser.hpp>
#include <AlgebraOutput.hpp>
#include <CppDataOutput.hpp>
#include <CppData.hpp>

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
