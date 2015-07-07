#include <pelib/CppData.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_CPPVECTOR
#define PELIB_CPPVECTOR

namespace pelib
{
	template <class Key, class Value>
	class CppVector: public CppData
	{
		typedef std::map<Key, Value> VectorType;
		
		public:
			CppVector() : CppData()
			{
				// Do nothing
			}
			virtual
			CppVector*
			clone() const
			{
				return new CppVector();
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Vector<Key, Value> *vector = dynamic_cast<const Vector<Key, Value>*>(data);
				if(vector == NULL) throw CastException("parameter \"data\" was not of type \"Vector<Key, Value>\".");
				
				// Dummy key and value and string for their type
				Key key;
				Value value;
				std::string key_str = type_name(typeid(key));
				std::string value_str = type_name(typeid(value));

				// Declare a map to contain values
				o << "std::map<" << key_str << ", " << value_str << "> map_" << vector->getName() << ";" << std::endl;

				// Fill the map with values
				std::map<Key, Value> map = vector->getValues();
				for(typename std::map<Key, Value>::iterator iter = map.begin(); iter != map.end(); iter++)
				{
					key = iter->first;
					value = iter->second;
					//key_str = type_name(typeid(key)).c_str();
					//value_str = type_name(typeid(value)).c_str();
					
					o << "map_" << vector->getName() << ".insert(std::pair<" << key_str.c_str() << ", " << value_str.c_str() << ">(" << key << ", " << value << "));" << std::endl;
				}
				
				o << "pelib::Vector<" << key_str << ", " << value_str << "> " << vector->getName() << "(\"" << vector->getName() << "\", map_" << vector->getName() << ");" << std::endl;	
			}

		protected:
		private:	
	};
}

#endif
