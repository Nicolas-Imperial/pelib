#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_RECORD
#define PELIB_RECORD
namespace pelib
{
	class Record
	{
		public:
		protected:
		// For some interesting reason, a pointer to Record pointing to a derived class is resolved as a Record by typeinfo if there is no virtual function in the base class.
		// This is an empty virtual function to make sure type_info can resolve the right class name
		virtual void do_nothing() {}
		private:
	};
}

#endif
