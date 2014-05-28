#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>
#include <set>

#include <AlgebraData.hpp>

#ifndef PELIB_SET
#define PELIB_SET

namespace pelib
{
	template <class Value> 
	class Set: public AlgebraData
	{
		typedef std::set<Value> SetType;
		
		public:
			Set(const std::string name, const SetType values) : AlgebraData(name), values(values)
			{
				// Do nothing
			}

			Set(const Set<Value>* Set): AlgebraData(Set->getName()), values(Set->getValues())
			{
				// Do nothing
			}

			virtual
			Set*
			clone() const
			{
				return new Set<Value>(name, values);
			}

			virtual
			const SetType&
			getValues() const
			{
				return values;
			}

			virtual
			bool
			find(Value elem) const
			{
				return getValues().find(elem) != getValues().end();
			}

			virtual
			size_t
			getSize() const
			{
				return values.size();
			}
			
		protected:
			SetType values;
		private:		
	};
}

#endif
