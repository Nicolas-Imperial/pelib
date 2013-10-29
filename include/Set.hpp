#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>
#include <set>

#include <Data.hpp>

#ifndef PELIB_SET
#define PELIB_SET

namespace pelib
{
	template <class Value> 
	class Set: public Data
	{
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			Set(const std::string name, const SetType values) : Data(name), values(values)
			{
				// Do nothing
			}

			Set(const Set<Value>* Set): Data(Set->getName()), values(Set->getValues())
			{
				// Do nothing
			}

			virtual
			Data*
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
				// TODO: implement
				return false;
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
