#include <sstream>

#include <pelib/AmplInputData.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/CastException.hpp>

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
			AmplInputMatrix(bool strict = true)
			{
				this->strict = strict;
			}
			
			virtual
			AmplInputMatrix*
			clone() const
			{
				return new AmplInputMatrix();
			}

			virtual
			std::string
			getDetailedPattern()
			{
				return "param\\s+([^\\s\\n:]+):\\s*(.*)\\s*:=(.+)";
			}

			virtual
			std::string
			getGlobalPattern()
			{
				return "param\\s+[^\\s\\n:]+:\\s*.*\\s*:=.+";
			}

			virtual
			AlgebraData*
			parse(std::istream &in)
			{
				MatrixType values;
				
				std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
				boost::cmatch match = AlgebraDataParser::match(getDetailedPattern(), str);
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
					try
					{
						cols.push_back(AlgebraDataParser::convert<Col>(*iter, strict));
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
				}
				int s = cols.size();

				// Parse the rest of the matrix; catch the row index every s reads
				iter = make_regex_token_iterator(remain, isolated_value, 1, boost::regex_constants::match_default);

				//int progress = 0;
				Row row;
				Col col;
				Value val;

				int integer_values = 0, total_values = 0;
				while(iter != end)
				{
					std::map<Col, Value> vector;

					try
					{
						row = AlgebraDataParser::convert<Row>(*iter, strict);
					} catch(NoDecimalFloatException &e)
					{
						std::ostringstream ss;
						ss << e.getValue();
						throw ParseException(std::string("Asked a decimal conversion, but \"").append(ss.str()).append("\" is integer."));		
					}
					
					iter++;
					for(int i = 0; i < s; i++)
					{
						try
						{
							val = AlgebraDataParser::convert<Value>(*iter, strict);
						} catch(NoDecimalFloatException &e)
						{
							float float_value = e.getValue();
							std::stringstream ss;

							ss << float_value;
							ss >> val;
							//val = Value(float_value);
							integer_values++;
						}
						
						col = cols[i];
						vector.insert(std::pair<Col, Value>(col, val));
						
						iter++;
						total_values++;
					}
					
					values.insert(std::pair<Row, RowType>(row, vector));
				}

				// If all values could have been parsed as integer, then this is obviously an integer vector rather to a float one
				if(integer_values == total_values)
				{
					//throw NoDecimalFloatException(std::string("Matrix only composed of integer-parsable values."), 0);
					throw ParseException(std::string("Matrix only composed of integer-parsable values."));
				}
				
				return new Matrix<Col, Row, Value>(match[1], values);
			}

			virtual
			void
			dump(std::ostream &o, const AlgebraData *data) const
			{			
				const Matrix<Col, Row, Value> *matrix = dynamic_cast<const Matrix<Col, Row, Value>*>(data);
				if(matrix == NULL) throw CastException("parameter \"data\" was not of type \"Matrix<Col, Row, Value>\".");
				MatrixType values = matrix->getValues();
				
				o << "param " << matrix->getName() << ":" << std::endl;
				typename std::map<Row, std::map<Col, Value> >::const_iterator row_iter = values.begin();

				if(row_iter != matrix->getValues().end())
				{
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
				}

				o << ";" << std::endl;
			}
	
		protected:
			bool strict;
		private:		
	};
}

#endif
