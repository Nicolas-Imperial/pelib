#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Data.hpp>

#ifndef PELIB_MATRIX
#define PELIB_MATRIX

namespace pelib
{
	template <class Col, class Row, class Value>
	class Matrix: public Data
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			Matrix(std::string name, MatrixType values) : Data(name)
			{
				this->values = values;
			}

			Matrix(Matrix<Col, Row, Value>* matrix): Data(matrix->getName()), values(matrix->getValues())
			{
				// Do nothing
			}

			virtual
			Data*
			clone()
			{
				return new Matrix<Col, Row, Value>(name, values);
			}

			virtual
			const MatrixType
			getValues() const
			{
				return values;
			}

			virtual
			const Value&
			find(Col col, Row row) const
			{
				return values.find(row)->second.find(col)->second;
			}

			virtual
			size_t
			getColSize() const // /!\ This is the size of a column, i.e. the number of rows
			{
				return values.size();
			}

			virtual
			size_t
			getRowSize() const // /!\ This is the size of a row, i.e. the number of columns
			{
				return values.begin()->second.size();
			}
			
		protected:
			MatrixType values;
		private:	
	};
}

#endif
