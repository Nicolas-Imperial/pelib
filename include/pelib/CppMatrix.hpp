#include <pelib/CppData.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/CastException.hpp>

#ifndef PELIB_CPPMATRIX
#define PELIB_CPPMATRIX

namespace pelib
{
	template <class Col, class Row, class Value>		
	class CppMatrix: public CppData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			CppMatrix() : CppData()
			{
				// Do nothing
			}
			
			virtual
			CppMatrix*
			clone() const
			{
				return new CppMatrix();
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{				
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");

				// Dummy key and value and string for their type
				Row row;
				Col col;
				Value value;
				std::string row_str = type_name(typeid(row)).c_str();
				std::string col_str = type_name(typeid(col)).c_str();
				std::string value_str = type_name(typeid(value)).c_str();

				// Declare a map to contain the whole matrix
				o << "std::map<" << row_str.c_str() << ", std::map<" << col_str.c_str() << ", " << value_str.c_str() << "> > map_" << matrix->getName() << ";" << std::endl;

				// Declare a map to contain each row
				o << "std::map<" << col_str.c_str() << ", " << value_str.c_str() << "> map_row;" << std::endl;
				for(typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = matrix->getValues().begin();
				    row_iter != matrix->getValues().end(); row_iter++)
				{
					row = row_iter->first;
					for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
					{
						col = col_iter->first;
						value = col_iter->second;

						// Add a value to row
						o << "map_row.insert(std::pair<" << col_str.c_str() << ", " << value_str.c_str() << ">(" << col << ", " << value << "));" << std::endl;
					}

					// Add the row to matrix
					o << "map_" << matrix->getName() << ".insert(std::pair<" << row_str.c_str()
						<< ", std::map<" << col_str.c_str() << ", " << value_str.c_str()
						<< "> >(" << row << ", map_row));" << std::endl;

					// Reinitialize row
					o << "map_row = std::map<" << col_str.c_str() << ", " << value_str.c_str() << ">();" << std::endl;
				}

				// Add matrix to data structure
				o << "pelib::Matrix<" << row_str.c_str() << ", " << col_str.c_str() << ", " << value_str.c_str() << "> " << matrix->getName() << "(\"" << matrix->getName() << "\", map_e);" << std::endl;  
			}
	
		protected:
		private:		
	};
}

#endif
