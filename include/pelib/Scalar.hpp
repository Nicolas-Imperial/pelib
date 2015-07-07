#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/AlgebraData.hpp>

#ifndef PELIB_SCALAR
#define PELIB_SCALAR

namespace pelib
{
	template <class Value>
	class Scalar: public AlgebraData
	{
		public:
			Scalar(const std::string name, const Value value) : AlgebraData(name)
			{
				this->value = value;
			}

			Scalar(const Scalar<Value>* scalar) : AlgebraData(scalar->getName())
			{
				// Do nothing
			}

			virtual
			Scalar*
			clone() const
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
