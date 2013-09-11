#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

using namespace std;
using namespace boost;

class ParseException: std::exception
{
	protected:
	string message;

	public:
	ParseException(string message) throw()
	{
		this->message = string(": ").append(message);
	}

	virtual
	~ParseException() throw() {}

	const char*
	what()
	{
		return string("Parsing exception").append(this->message).c_str();
	}
};

class AmplRecord
{
	protected:
	string name;

	static
	boost::cmatch
	match(string regex, string str)
	{
		boost::cmatch match;
		boost::regex param_scalar(regex);

		if(boost::regex_match(str.c_str(), match, param_scalar))
		{
			return match;
		}
		else
		{
			throw ParseException("Text doesn't match regex");
		}
	}

	template <class Target>
	static
	Target
	convert(string element)
	{
		Target out;
		istringstream converter(element);
		converter >> out;

		if(converter.fail())
		{
			throw ParseException("Couln't convert literal into desired type");
		}

		return out;
	}

	public:
	virtual std::ostream&
	dump(std::ostream& o) const = 0;

	virtual
	void
	parse(istream &in) = 0;

	string
	getName() const
	{
		return name;
	}

	virtual
	AmplRecord*
	clone() const = 0;
};

std::ostream&
operator<<(std::ostream& o, const AmplRecord& r)
{
	return r.dump(o);
}

template <class Value>
class
ParamScalar: public AmplRecord
{
	protected:
	Value value;

	public:
	ParamScalar(){}
	ParamScalar(const ParamScalar<Value> *scalar) : value(scalar->value) {}

	void
	parse(istream &in)
	{	
		string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		boost::cmatch match = AmplRecord::match("^param\\s+([^\\s\n]*?)\\s*:=\\s*([^\\s\n]*?)\\s*$", str);

		this->name = match[1];
		this->value = AmplRecord::convert<Value>(match[2]);
	}

	Value
	getValue() const
	{
		return this->value;
	}

	virtual
	ParamScalar*
	clone() const
	{
		return new ParamScalar(this);
	}

	virtual
	std::ostream&
	dump(std::ostream& o) const
	{
		return o << "param " << this->name << " := " << this->value << ";" << endl;
	}
};

template <class Key, class Value>
class
ParamVector: public AmplRecord
{
	protected:
	std::map<Key, Value> values;
	
	public:
	ParamVector(){}
	ParamVector(const ParamVector<Key, Value> *vector) : values(vector->values) {}

	void
	parse(istream &in)
	{
		string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		boost::cmatch match=AmplRecord::match("^param\\s*:\\s*([^\\s\\n]+)\\s*:=(.*)$", str);
		this->name = match[1];

		boost::regex param_vector("(?:\\s*([^\\s]+)\\s+([^\\s]+))");
		string remain = match[2];
		const int subs[] = {1, 2};
		boost::sregex_token_iterator iter = make_regex_token_iterator(remain, param_vector, subs, regex_constants::match_default);
		boost::sregex_token_iterator end;

		for(; iter != end; ++iter )
		{
			Key key;
			Value value;

			key = AmplRecord::convert<Value>(*iter++);
			value = AmplRecord::convert<Value>(*iter);

			values.insert(std::pair<Key, Value>(key, value));
		}
	}

	virtual
	map<Key, Value>
	getValues() const
	{
		return values;
	}

	virtual
	Value
	find(Key key)
	{
		return values.find(key)->second;
	}

	virtual
	size_t
	getSize()
	{
		return values.size();
	}

	virtual
	ParamVector*
	clone() const
	{
		return new ParamVector(this);
	}

	virtual
	std::ostream&
	dump(std::ostream& o) const 
	{
		o << "param: " << this->name << " :=" << endl;
		for(typename map<Key, Value>::const_iterator iter = values.begin(); iter != values.end(); iter++)
		{
			o << iter->first << " " << iter->second << endl;
		}
		return o << ";" << endl;
	}
};

template <class Row, class Col, class Value>
class
ParamMatrix: public AmplRecord
{
	typedef map<Col, Value> RowType;
	typedef map<Row, RowType> MatrixType;

	protected:
	MatrixType values;

	public:
	ParamMatrix(){}
	ParamMatrix(const ParamMatrix<Row, Col, Value> *matrix) : values(matrix->values) {}

	void
	parse(istream &in)
	{
		string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		boost::cmatch match = AmplRecord::match("param\\s+([^\\s:]*):\\s+(.*?)\\s*:=(.*)", str);
		this->name = match[1];
		string indexes = match[2];
		string remain = match[3];

		// Extract all indexes
		boost::regex isolated_value("\\s*([^\\s]+)\\s*");
		boost::sregex_token_iterator iter = make_regex_token_iterator(indexes, isolated_value, 1, regex_constants::match_default);
		boost::sregex_token_iterator end;
		
		// Put them in a vector
		vector<Col> cols;
		for(; iter != end; ++iter)
		{
			cols.push_back(AmplRecord::convert<Col>(*iter));
		}
		int s = cols.size();

		// Parse the rest of the matrix; catch the row index every s reads
		iter = make_regex_token_iterator(remain, isolated_value, 1, regex_constants::match_default);

		int progress = 0;
		Row row;
		Col col;
		Value val;
		while(iter != end)
		{
			map<Col, Value> vector;

			row = AmplRecord::convert<Row>(*iter);
			iter++;
			for(int i = 0; i < s; i++)
			{
				val = AmplRecord::convert<Value>(*iter);
				col = cols[i];
				vector.insert(pair<Col, Value>(col, val));
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
		o << "param " << this->name << ":" << endl;
		typename map<Row, map<Col, Value> >::const_iterator row_iter = values.begin();

		for(typename map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
		{
			o << col_iter->first << " ";
		}
		o << ":=" << endl;
		
		for(row_iter = values.begin(); row_iter != values.end(); row_iter++)
		{
			o << row_iter->first << " ";
			for(typename map<Col, Value>::const_iterator col_iter = row_iter->second.begin(); col_iter != row_iter->second.end(); col_iter++)
			{
				o << fixed << setprecision(6) << col_iter->second << " ";
			}
			o << endl;
		}
		return o << ";" << endl;
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

class AmplData
{
	protected:
	map<string, AmplRecord*> records;
	vector<AmplRecord*> default_parsers;

	public:
	AmplData()
	{
		default_parsers.push_back(new ParamScalar<int>());
		default_parsers.push_back(new ParamVector<int, int>());
		default_parsers.push_back(new ParamMatrix<int, int, float>());
	}

	AmplData(vector<AmplRecord*> parsers)
	{
		default_parsers = parsers;
	}	

	virtual
	~AmplData()
	{
		for(vector<AmplRecord*>::iterator i = default_parsers.begin(); i != default_parsers.end(); i++)
		{
			delete *i;
		}

		for (map<string, AmplRecord*>::iterator i = records.begin(); i != records.end(); i++)
		{
			delete i->second;
		}
	}		

	map<string, AmplRecord*>
	getAllRecords()
	{
		return records;
	}

	template<class Record>
	map<string, Record*>
	getRecords()
	{
		// Build a new map containing all elements from records that could be casted to Record
//		cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << "Hello world" << endl;
		map<string, Record*> record;

		for (map<string, AmplRecord*>::iterator i = records.begin(); i != records.end(); i++)
		{
			try
			{
				Record* elem = dynamic_cast<Record*>(i->second);
				record.insert(pair<string, Record*>(i->first, elem));
			} catch(std::exception &e)
			{
				// Couldn't cast the element to record: just let that go and try again with next element
			}
		}

		return record;
	}

	template<class Record>
	Record*
	find(string key)
	{
		map<string, Record* > rec = this->getRecords<Record>();
		return rec.find(key)->second;
	}

	void
	parse(istream &ampl_data)
	{
		parse(ampl_data, this->default_parsers);
	}

	void
	parse(istream &ampl_data, vector<AmplRecord*> parsers)
	{
		string line, output, &out=output;
		string first_only = "\\1";
		string nothing = "";

		while(!getline(ampl_data, line, ';').fail())
		{
			boost::regex surrounding_space("^[\n\\s]*(.*?)[\\s\n]*$");
			boost::regex comment("(.*?)#.*?(\n|$)");
			line = boost::regex_replace(line, comment, first_only, boost::match_default | boost::format_all);
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.length() == 0)
			{
				continue;
			}

			vector<AmplRecord*>::iterator iter;
			for(iter = parsers.begin(); iter != parsers.end(); iter++)
			{
				AmplRecord *parser = *iter;
				AmplRecord *record;
				try {
//					cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << line << endl;
					record = parser->clone();
					std::istringstream istr(line);
					std::istream &str = istr;
					record->parse(str);
					records.insert(pair<string, AmplRecord*>(record->getName(), record));

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
					delete record;
				}
			}

			if(iter == parsers.end())
			{
				// No vector
				throw ParseException(string("No parser was suitable to the token \"").append(line).append("\"."));
			}
		}
	}

	virtual std::ostream&
	dump(std::ostream& o) const
	{
		for (map<string, AmplRecord*>::const_iterator i = records.begin(); i != records.end(); i++)
		{
			o << *(i->second) << endl;
		}

		return o;
	}
};

std::ostream&
operator<<(std::ostream& o, const AmplData& r)
{
	
	return r.dump(o);
}

int
main(int argc, char **argv)
{
	ifstream myfile;
	myfile.open (argv[1], ios::in);
	AmplData data;

	try {
		data.parse(myfile);
	} catch(ParseException &e)
	{
		cerr << e.what() << endl;
	}
	myfile.close();

	map<string, ParamScalar<int>* > scalarRecords = data.getRecords<ParamScalar<int> >();
	ParamScalar<int>* n = scalarRecords.find("n")->second;
	map<string, ParamVector<int, int>* > vectorRecords = data.getRecords<ParamVector<int, int> >();
	ParamVector<int, int>* Wi = vectorRecords.find("Wi")->second;

	cout << data << endl;

	cout << data.find<ParamMatrix<int, int, float> >("e")->getColSize() << endl;
	
	return EXIT_SUCCESS;
}
