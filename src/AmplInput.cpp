#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplInputScalar.hpp>
#include <AmplInputVector.hpp>
#include <AmplInputSet.hpp>
#include <AmplInputMatrix.hpp>
#include <AmplDataParser.hpp>

#include <ParseException.hpp>
#include <CastException.hpp>

namespace pelib
{
	AmplInput::AmplInput()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	AmplInput::AmplInput(std::vector<AmplInputData*> parsers,
			std::vector<AmplInputData*> outputs)
	{
		this->parsers = parsers;
		this->outputs = outputs;
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

		return *this;
	}

	Record
	AmplInput::parse(std::istream &ampl_data)
	{
		Record record;
		std::string line;
		std::string first_only = "\\1";
		std::string nothing = "";

		// Remove all comments from input before starting to parse it
		std::stringstream noComment;
		while(!getline(ampl_data, line).fail())
		{
			boost::regex comment("([^#]*)#[^$]*$");
            line = boost::regex_replace(line, comment, first_only, boost::match_default | boost::format_all);
			boost::regex surrounding_space("^[\\n\\s]*([^\\s\n]*)[\\s\n]*$");
			line = boost::regex_replace(line, surrounding_space, first_only, boost::match_default | boost::format_all);

			if(line.compare("") != 0)
			{
				noComment << line << std::endl;
			}
		}
		
		// Parse input
		while(!getline(noComment, line, ';').fail())
		{
			boost::regex surrounding_space("^[\\n\\s]*([^\\s\\n].*?)[\\s\\n]*$");
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
					Data *data = parser->parse(str);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
					//std::cerr << e.what() << "Trying next parser." << std::endl;
				}
			}

			if(iter == parsers.end())
			{
				//throw ParseException(std::string("No parser was suitable to the token \"").append(line).append("\"."));
			}
		}
		
		return record;
	}

	void
	AmplInput::dump(std::ostream& o, const Record &record) const
	{
		const std::map<std::string, const Data * const> records = record.getAllRecords();
		for (std::map<std::string, const Data * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			dump(o, rec->second);
		}
	}

	void
	AmplInput::dump(std::ostream& o, const Data *data) const
	{
		for (std::vector<AmplInputData*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
		{
			const DataOutput *output = *out;
			try
			{
				output->dump(o, data);
				break;
			} catch(CastException &e)
			{
				// No suitable element to output
				// Couldn't cast the element to record: just let that go and try again with next element
			}
		}
	}

	// Protected
	void
	AmplInput::addParsers()
	{		
		parsers.push_back(new AmplInputScalar<int>(true));
		parsers.push_back(new AmplInputScalar<float>(true));
		parsers.push_back(new AmplInputVector<int, int>(true));
		parsers.push_back(new AmplInputVector<int, float>(true));
		parsers.push_back(new AmplInputSet<int>(true));
		parsers.push_back(new AmplInputSet<float>(true));
		parsers.push_back(new AmplInputMatrix<int, int, int>(true));
		parsers.push_back(new AmplInputMatrix<int, int, float>(true));
	}

	void			
	AmplInput::addOutputs()
	{		
		outputs.push_back(new AmplInputScalar<int>(true));
		outputs.push_back(new AmplInputScalar<float>(true));
		outputs.push_back(new AmplInputVector<int, int>(true));
		outputs.push_back(new AmplInputVector<int, float>(true));
		outputs.push_back(new AmplInputSet<int>(true));
		outputs.push_back(new AmplInputSet<float>(true));
		outputs.push_back(new AmplInputMatrix<int, int, int>(true));
		outputs.push_back(new AmplInputMatrix<int, int, float>(true));
	}
}
