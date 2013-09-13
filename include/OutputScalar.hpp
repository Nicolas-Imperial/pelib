#include <ostream>

#include <AmplRecordOutput.hpp>
#include <ParamScalar.hpp>

#ifndef OUTPUTSCALAR
#define OUTPUTSCALAR

namespace pelib
{
	template <class Value>
	class OutputScalar : public AmplRecordOutput
	{
		public:
		Value
		getValue()
		{
			return this->values;
		}

		OutputScalar() {} // Empty constructor
		OutputScalar(const std::string name, const Value value = 0) // Build from a map
		{
			this->name = name;
			this->value = value;
		}

		OutputScalar(const ParamScalar<Value>* scalar)
		{
			this->name = scalar->getName();
			this->value = scalar->getValue();
		}

		OutputScalar(const OutputScalar<Value>* scalar) // copy constructor
		{
			this->name = scalar->name;
			this->value = scalar->value;
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const
		{
			return o << this->name << " = " << this->value << std::endl;
		}

		OutputScalar*
		clone() const
		{
			return new OutputScalar(this);
		}

		void
		setName(std::string name)
		{
			this->name = name;
		}

		protected:
		Value value;

		private:
	};
}

#endif
