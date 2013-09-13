#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#ifndef AMPLRECORD
#define AMPLRECORD

namespace pelib
{
	class AmplRecord
	{
		protected:
		std::string name;

		public:
		virtual
		~AmplRecord();

		virtual std::ostream&
		dump(std::ostream& o) const = 0;

		std::string
		getName() const;

		virtual
		AmplRecord*
		clone() const = 0;
	};

	std::ostream&
	operator<<(std::ostream& o, const AmplRecord& r);
}

#endif

