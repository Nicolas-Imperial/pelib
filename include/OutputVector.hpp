#include <map>
#include <ostream>
#include <typeinfo>

#include <AmplRecordOutput.hpp>
#include <ParamVector.hpp>

#ifndef OUTPUTVECTOR
#define OUTPUTVECTOR

namespace pelib
{
	template <class Key, class Value>
	class OutputVector : public AmplRecordOutput
	{
		typedef std::pair<Key, Value> output_vector_pair_t;
		typedef std::map<Key, Value> output_vector_map_t;

		public:
		output_vector_map_t
		getValues()
		{
			return this->values;
		}

		OutputVector() {} // Empty constructor
		OutputVector(const std::string name, const output_vector_map_t& values = NULL) // Build from a map
		{
			this->name = name;
			this->values = values;
		}

		OutputVector(const ParamVector<Key, Value>* vector)
		{
			this->name = vector->getName();
			this->values = vector->getValues();
		}

		OutputVector(const OutputVector<Key, Value>* vector) // copy constructor
		{
			this->name = vector->name;
			this->values = vector->values;
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const
		{
			o << this->name << " [*] := " << std::endl;

			for(typename std::map<Key, Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
			{
				o << iter->first << " " << iter->second << std::endl;
			}
			o << ";" << std::endl;

			return o;
		}

		OutputVector*
		clone() const
		{
			return new OutputVector(this);
		}

		protected:
		output_vector_map_t values;

		private:
	};
}

#endif
