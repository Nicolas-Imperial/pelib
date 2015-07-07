#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/AlgebraData.hpp>

#ifndef PELIB_VECTOR
#define PELIB_VECTOR

namespace pelib
{
	template <class Key, class Value> 
	class Vector: public AlgebraData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			Vector(const std::string name, const VectorType values) : AlgebraData(name), values(values)
			{
				// Do nothing
			}

			Vector(const Vector<Key, Value>* vector): AlgebraData(vector->getName()), values(vector->getValues())
			{
				// Do nothing
			}

			virtual
			Vector*
			clone() const
			{
				return new Vector<Key, Value>(name, values);
			}

			virtual
			const std::map<Key, Value>&
			getValues() const
			{
				return values;
			}

			virtual
			const Value&
			find(Key key) const
			{
				return values.find(key)->second;
			}

			virtual
			size_t
			getSize() const
			{
				return values.size();
			}
			
		protected:
			VectorType values;
		private:		
	};
}

#endif
