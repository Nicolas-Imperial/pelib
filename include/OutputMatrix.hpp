#include <map>
#include <ostream>
#include <typeinfo>

#include <AmplRecordOutput.hpp>
#include <ParamMatrix.hpp>

#ifndef OUTPUTMATRIX
#define OUTPUTMATRIX

namespace pelib
{
	template <class Row, class Col, class Value>
	class OutputMatrix : public AmplRecordOutput
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;

		public:
		MatrixType
		getValues()
		{
			return this->values;
		}

		OutputMatrix() {} // Empty constructor
		OutputMatrix(const std::string name, const MatrixType& values = NULL) // Build from a map
		{
			this->name = name;
			this->values = values;
		}

		OutputMatrix(const ParamMatrix<Row, Col, Value>* matrix)
		{
			this->name = matrix->getName();
			this->values = matrix->getValues();
		}

		OutputMatrix(const OutputMatrix<Row, Col, Value>* matrix) // copy constructor
		{
			this->name = matrix->name;
			this->values = matrix->values;
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const
		{
			o << this->name << " := " << std::endl;
			typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = values.begin();
			for(; row_iter != values.end(); row_iter++)
			{
				for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
				{	
					o << row_iter->first << " " << col_iter->first << " " << col_iter->second << std::endl;
				}
			}
			return o << ";" << std::endl;
		}

		OutputMatrix*
		clone() const
		{
			return new OutputMatrix(this);
		}

		void
		setName(std::string name)
		{
			this->name = name;
		}

		protected:
		MatrixType values;

		private:
	};
}

#endif
