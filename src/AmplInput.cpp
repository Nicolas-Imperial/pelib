#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplInputScalar.hpp>
#include <AmplInputVector.hpp>
#include <AmplInputMatrix.hpp>
#include <AmplDataParser.hpp>

#include <ParseException.hpp>
#include <CastException.hpp>

namespace pelib
{
	AmplInput::AmplInput()
	{
		// Add parsers
		parsers.push_back(new AmplInputScalar<int>());
		parsers.push_back(new AmplInputVector<int, int>());
		parsers.push_back(new AmplInputMatrix<int, int, float>());
				
		// Add interpreters
		outputs.push_back(new AmplInputScalar<int>());
		outputs.push_back(new AmplInputVector<int, int>());
		outputs.push_back(new AmplInputMatrix<int, int, float>());
	}

	AmplInput::~AmplInput()
	{
		deleteParsers();
		deleteOutputs();			
	}
	
	void
	AmplInput::deleteParsers()
	{
		
		for(std::vector<AmplInputData*>::iterator i = parsers.begin(); i != parsers.end(); i = parsers.erase(i))
		{
			delete *i;
		}
	}

	void
	AmplInput::deleteOutputs()
	{
		for(std::vector<AmplInputData*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	AmplInput&
	AmplInput::operator=(const AmplInput &amplInput)
	{
		deleteParsers();
		deleteOutputs();

		for(std::vector<AmplInputData*>::const_iterator i = amplInput.parsers.begin(); i != amplInput.parsers.end(); i++)
		{
			parsers.push_back((*i)->clone());
		}

		for(std::vector<AmplInputData*>::const_iterator i = amplInput.outputs.begin(); i != amplInput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}
	}

	Record
	AmplInput::parse(std::istream &ampl_data, bool strict)
	{
		Record record;
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
				//std::cerr << line << std::endl;
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

			std::vector<AmplInputData*>::iterator iter;
			
			for(iter = parsers.begin(); iter != parsers.end(); iter++)
			{
				AmplInputData *parser = *iter;
				try {
					std::istringstream istr(line);
					std::istream &str = istr;
					Data *data = parser->parse(str, strict);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
					// Try next parser
				}
			}

			if(iter == parsers.end())
			{
				// No vector
				throw ParseException(std::string("No parser was suitable to the token \"").append(line).append("\"."));
			}
		}
		
		return record;
	}

	std::ostream&
	AmplInput::dump(std::ostream& o, const Record &record) const
	{
		std::map<std::string, Data*> records = record.getAllRecords();
		for (std::map<std::string, Data*>::iterator rec = records.begin(); rec != records.end(); rec++)
		{
			Data *record = rec->second;
			for (std::vector<AmplInputData*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
			{
				const DataOutput *output = *out;
				try
				{
					output->dump(o, record);
					break;
				} catch(CastException &e)
				{
					// No suitable element to output
					// Couldn't cast the element to record: just let that go and try again with next element
				}
			}
		}
	}
}