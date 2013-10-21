#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_SCALAR
#define PELIB_SCALAR

namespace pelib
{
	template <class Value>
	class Scalar: public Data
	{
		public:
			Scalar(std::string name, Value value) : Data(name)
			{
				this->value = value;
			}

			Scalar(Scalar<Value>* scalar): Data(scalar->getName()), value(scalar->getValue())
			{
				// Do nothing
			}

			virtual
			Scalar*
			clone()
			{
				return new Scalar<Value>(name, value);
			}

			virtual
			const Value&
			getValue() const
			{
				return value;
			}
				
		protected:
			Value value;
		private:		
	};
}

#endif
