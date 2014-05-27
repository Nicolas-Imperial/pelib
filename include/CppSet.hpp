#include <set>

#include <CppData.hpp>
#include <Set.hpp>
#include <Scalar.hpp>
#include <CastException.hpp>

#ifndef PELIB_CPPSET
#define PELIB_CPPSET

namespace pelib
{
	template <class Value>
	class CppSet: public CppData
	{
		typedef std::set<Value, std::less<Value>, std::allocator<Value> > SetType;
		
		public:
			CppSet() : CppData()
			{
				// Do nothing
			}
			
			virtual
			CppSet*
			clone() const
			{
				return new CppSet();
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Set<Value> *set = dynamic_cast<const Set<Value>*>(data);
				if(set == NULL) throw CastException("parameter \"data\" was not of type \"Set<Value>\".");
				
				// Dummy key and value and string for their type
				Value value;
				std::string value_str = type_name(typeid(value));

				// Declare a map to contain values
				o << "std::set<" << value_str.c_str() << "> set_" << set->getName() << ";" << std::endl;

				// Fill the map with values
				std::set<Value> stdset = set->getValues();
				for(typename std::set<Value>::iterator iter = stdset.begin(); iter != stdset.end(); iter++)
				{
					value = *iter;
					//value_str = type_name(typeid(value)).c_str();
					
					o << "set_" << set->getName() << ".insert(" << value << ");" << std::endl;
				}
				
				o << "pelib::Set<" << value_str.c_str() << "> " << set->getName() << "(\"" << set->getName() << "\", set_" << set->getName() << ");" << std::endl;
			}

		protected:
		private:	
	};
}

#endif
