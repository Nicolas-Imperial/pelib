#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_VECTOR
#define PELIB_VECTOR

namespace pelib
{
	template <class Key, class Value> 
	class Vector: public Data
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			Vector(std::string name, VectorType values) : Data(name), values(values)
			{
				//this->values = values;
			}

			Vector(Vector<Key, Value>* vector): Data(vector->getName()), values(vector->getValues())
			{
				// Do nothing
			}

			virtual
			Data*
			clone()
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
