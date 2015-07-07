#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <pelib/AlgebraData.hpp>

#ifndef PELIB_MATRIX
#define PELIB_MATRIX

namespace pelib
{
	template <class Col, class Row, class Value>
	class Matrix: public AlgebraData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:
			Matrix(const std::string name, const MatrixType values) : AlgebraData(name)
			{
				this->values = values;
			}

			Matrix(const Matrix<Col, Row, Value>* matrix): AlgebraData(matrix->getName()), values(matrix->getValues())
			{
				// Do nothing
			}

			virtual
			Matrix*
			clone() const
			{
				return new Matrix<Col, Row, Value>(name, values);
			}

			virtual
			const MatrixType&
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

			virtual
			Matrix<Row, Col, Value>
			transpose() const
			{
				std::map<Col, std::map<Row, Value> > new_values;
				for(typename MatrixType::const_iterator i = values.begin(); i != values.end(); i++)
				{
					for(typename RowType::const_iterator j = i->second.begin(); j != i->second.end(); j++)
					{
						Value val = j->second;
						new_values[j->first][i->first] = val;
					}
				}

				return Matrix<Row, Col, Value>(this->getName(), new_values);
			}
			
		protected:
			MatrixType values;
		private:	
	};
}

#endif
