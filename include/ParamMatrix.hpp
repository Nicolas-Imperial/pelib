#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplRecordParser.hpp>

#ifndef PARAMMATRIX
#define PARAMMATRIX

namespace pelib
{
	template <class Row, class Col, class Value>
	class
	ParamMatrix: public AmplRecordParser
	{
		typedef std::map<Col, Value> RowType;
		typedef std::map<Row, RowType> MatrixType;

		protected:
		MatrixType values;

		public:
		ParamMatrix(){}
		ParamMatrix(const ParamMatrix<Row, Col, Value> *matrix) : values(matrix->values) {}

		void
		parse(std::istream &in)
		{
			std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			boost::cmatch match = AmplRecordParser::match("param\\s+([^\\s:]*):\\s+(.*?)\\s*:=(.*)", str);
			this->name = match[1];
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
				cols.push_back(AmplRecordParser::convert<Col>(*iter));
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

				row = AmplRecordParser::convert<Row>(*iter);
				iter++;
				for(int i = 0; i < s; i++)
				{
					val = AmplRecordParser::convert<Value>(*iter);
					col = cols[i];
					vector.insert(std::pair<Col, Value>(col, val));
					iter++;	
				}
				values.insert(std::pair<Row, RowType>(row, vector));
			}
		}

		virtual
		MatrixType
		getValues() const
		{
			return values;
		}

		virtual
		ParamMatrix*
		clone() const
		{
			return new ParamMatrix(this);
		}

		virtual
		std::ostream&
		dump(std::ostream& o) const
		{
			o << "param " << this->name << ":" << std::endl;
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
//					o << std::fixed << std::setprecision(6) << col_iter->second << " ";
					o << col_iter->second << " ";
				}
				o << std::endl;
			}
			return o << ";" << std::endl;
		}

		virtual
		Value
		find(Col col, Row row)
		{
			return values.find(row)->second.find(col)->second;
		}

		virtual
		size_t
		getColSize() // /!\ This is the size of a column, i.e. the number of rows
		{
			return values.size();
		}

		virtual
		size_t
		getRowSize() // /!\ This is the size of a row, i.e. the number of columns
		{
			return values.begin()->second.size();
		}
	};
}

#endif
