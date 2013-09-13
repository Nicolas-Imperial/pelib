#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplData.hpp>
#include <ParamScalar.hpp>
#include <ParamVector.hpp>
#include <ParamMatrix.hpp>

namespace pelib
{
	AmplData::AmplData()
	{
		default_parsers.push_back(new ParamScalar<int>());
		default_parsers.push_back(new ParamVector<int, int>());
		default_parsers.push_back(new ParamMatrix<int, int, float>());
	}

	AmplData::AmplData(std::vector<AmplRecordParser*> parsers)
	{
		default_parsers = parsers;
	}	

	AmplData::~AmplData()
	{
		for(std::vector<AmplRecordParser*>::iterator i = default_parsers.begin(); i != default_parsers.end(); i++)
		{
			delete *i;
		}
			for (std::map<std::string, AmplRecordParser*>::iterator i = records.begin(); i != records.end(); i++)
		{
			delete i->second;
		}
	}		

	std::map<std::string, AmplRecordParser*>
	AmplData::getAllRecords()
	{
		return records;
	}

	void
	AmplData::parse(std::istream &ampl_data)
	{
		parse(ampl_data, this->default_parsers);
	}

	void
	AmplData::parse(std::istream &ampl_data, std::vector<AmplRecordParser*> parsers)
	{
		std::string line;
		std::string first_only = "\\1";
		std::string nothing = "";

		// Remove all comments from input before starting to parse it
		std::stringstream noComment;
		while(!getline(ampl_data, line).fail())
		{
			boost::regex comment("(.*?)#.*?$");
                        line = boost::regex_replace(line, comment, first_only, boost::match_default | boost::format_all);
			boost::regex surrounding_space("^[\n\\s]*(.*?)[\\s\n]*$");
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.compare("") != 0)
			{
				noComment << line << std::endl;
			}
		}

		// Parse input
		while(!getline(noComment, line, ';').fail())
		{
			boost::regex surrounding_space("^[\n\\s]*(.*?)[\\s\n]*$");
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.length() == 0)
			{
				continue;
			}

			std::vector<AmplRecordParser*>::iterator iter;
			for(iter = parsers.begin(); iter != parsers.end(); iter++)
			{
				AmplRecordParser *parser = *iter;
				AmplRecordParser *record;
				try {
//					cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << line << endl;
					record = parser->clone();
					std::istringstream istr(line);
					std::istream &str = istr;
					record->parse(str);
					records.insert(std::pair<std::string, AmplRecordParser*>(record->getName(), record));

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
				throw ParseException(std::string("No parser was suitable to the token \"").append(line).append("\"."));
			}
		}
	}

	std::ostream&
	AmplData::dump(std::ostream& o) const
	{
		for (std::map<std::string, AmplRecordParser*>::const_iterator i = records.begin(); i != records.end(); i++)
		{
			o << *(i->second) << std::endl;
		}
		return o;
	}

	std::ostream&
	operator<<(std::ostream& o, const AmplData& r)
	{
		return r.dump(o);
	}
}
