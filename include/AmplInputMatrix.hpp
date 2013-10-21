#include <AmplInputData.hpp>
#include <Matrix.hpp>
#include <CastException.hpp>

#ifndef PELIB_AMPLINPUTMATRIX
#define PELIB_AMPLINPUTMATRIX

namespace pelib
{
	template <class Col, class Row, class Value>		
	class AmplInputMatrix: public AmplInputData
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;
		
		public:			
			virtual
			AmplInputMatrix*
			clone() const
			{
				return new AmplInputMatrix();
			}

			virtual
			std::string
			getPattern()
			{
				return "param\\s+([^\\s:]*):\\s+(.*?)\\s*:=(.*)";
			}

			virtual
			Data*
			parse(std::istream &in, bool strict = 0)
			{
				MatrixType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match = DataParser::match(getPattern(), str);
				std::string indexes = match[2];
				std::string remain = match[3];

				// Extract all indexes
				boost::regex isolated_value("\\s*([^\\s]+)\\s*");
				boost::sregex_token_iterator iter = make_regex_token_iterator(indexes, isolated_value, 1, boost::regex_constants::match_default);
				boost::sregex_token_iterator end;
			
				// Put them in a vector
				std::vector<Col> cols;
				for(; iter != end; ++iter)
				{
					cols.push_back(DataParser::convert<Col>(*iter, strict));
				}
				int s = cols.size();

				// Parse the rest of the matrix; catch the row index every s reads
				iter = make_regex_token_iterator(remain, isolated_value, 1, boost::regex_constants::match_default);

				int progress = 0;
				Row row;
				Col col;
				Value val;
				while(iter != end)
				{
					std::map<Col, Value> vector;

					row = DataParser::convert<Row>(*iter, strict);
					iter++;
					for(int i = 0; i < s; i++)
					{
						val = DataParser::convert<Value>(*iter, strict);
						col = cols[i];
						vector.insert(std::pair<Col, Value>(col, val));
						iter++;	
					}
					values.insert(std::pair<Row, RowType>(row, vector));
				}
				
				return new Matrix<Col, Row, Value>(match[1], values);
			}

			virtual
			std::ostream&
			dump(std::ostream &o, const Data *data) const
			{			
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");
				MatrixType values = matrix->getValues();
				
				o << "param " << matrix->getName() << ":" << std::endl;
				typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = values.begin();

				for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
				{
					o << col_iter->first << " ";
				}
				o << ":=" << std::endl;
		
				for(row_iter = values.begin(); row_iter != values.end(); row_iter++)
				{
					o << row_iter->first << " ";
					for(typename std::map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
					{
						o << col_iter->second << " ";
					}
					o << std::endl;
				}
				return o << ";" << std::endl;
			}
	
		protected:
		private:		
	};
}

#endif
