#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplOutput.hpp>
#include <AmplOutputScalar.hpp>
#include <AmplOutputVector.hpp>
#include <AmplOutputSet.hpp>
#include <AmplOutputMatrix.hpp>
#include <AmplDataParser.hpp>

#include <ParseException.hpp>
#include <CastException.hpp>

namespace pelib
{
	AmplOutput::AmplOutput()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	AmplOutput::AmplOutput(std::vector<AmplOutputData*> parsers,
			std::vector<AmplOutputData*> outputs)
	{
		this->parsers = parsers;
		this->outputs = outputs;
	}

	AmplOutput::~AmplOutput()
	{
		deleteParsers();
		deleteOutputs();			
	}

	void
	AmplOutput::deleteParsers()
	{
		for(std::vector<AmplOutputData*>::iterator i = parsers.begin(); i != parsers.end(); i = parsers.erase(i))
		{
			delete *i;
		}
	}

	void
	AmplOutput::deleteOutputs()
	{
		for(std::vector<AmplOutputData*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	AmplOutput&
	AmplOutput::operator=(const AmplOutput &amplOutput)
	{
		deleteParsers();
		deleteOutputs();

		for(std::vector<AmplOutputData*>::const_iterator i = amplOutput.parsers.begin(); i != amplOutput.parsers.end(); i++)
		{
			parsers.push_back((*i)->clone());
		}

		for(std::vector<AmplOutputData*>::const_iterator i = amplOutput.outputs.begin(); i != amplOutput.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	Record
	AmplOutput::parse(std::istream &ampl_data)
	{
		Record record;
		std::string line;
		
		while(!getline(ampl_data, line, ';').fail())
		{
			// Read section into string buffer stream
			std::stringstream section;
			section << line;

			while(!getline(section, line, ';').fail())
			{			
				std::vector<AmplOutputData*>::iterator iter;
				for(iter = parsers.begin(); iter != parsers.end(); iter++)
				{
					AmplOutputData *parser = *iter;
					try {
						// If any non-blank prefix to be discarded, it must be separated by a new line
						std::string regex = std::string("(.*?\\n|)\\s*(")
							.append(parser->getGlobalPattern())
							.append(")(?:\\s*)");
						
						boost::cmatch match = DataParser::match(regex, line);

						std::stringstream token;
						token.str(match[2]);

						//std::cerr << "Discarded =\"" << match[1] << "\"." << std::endl;
						//std::cerr << "Token =\"" << match[2] << "\"." << std::endl;
						Data *data = parser->parse(token);
						record.insert(data);

						// Keep only the part we have not parsed
						line = match[1];
						
						// Feed the remaining input line to parsers input is more useful information are available
	  					section.str(line);
						section.seekp(std::ios_base::beg);
						section.clear();

						// No need to try another parser; proceed with the next token in section
						break;
					} catch (ParseException &e)
					{
						// Try next parser
					}
				}

				// There was no parser for this section; drop it and go to next
				if(iter == parsers.end())
				{
					// Break this loop and read another text section
					break;
				}
			}
		}
		
		return record;
	}

	void
	AmplOutput::dump(std::ostream& o, const Record &record) const
	{
		std::map<std::string, const Data * const> records = record.getAllRecords();
		for (std::map<std::string, const Data * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
		{
			dump(o, rec->second);
		}
	}

	void
	AmplOutput::dump(std::ostream& o, const Data *data) const
	{
		for (std::vector<AmplOutputData*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
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
	AmplOutput::addParsers()
	{		
		parsers.push_back(new AmplOutputScalar<int>(true));
		parsers.push_back(new AmplOutputScalar<float>(true));
		parsers.push_back(new AmplOutputVector<int, int>());
		parsers.push_back(new AmplOutputVector<int, float>());
		parsers.push_back(new AmplOutputSet<int>());
		parsers.push_back(new AmplOutputSet<float>());
		parsers.push_back(new AmplOutputMatrix<int, int, int>());
		parsers.push_back(new AmplOutputMatrix<int, int, float>());
	}

	void			
	AmplOutput::addOutputs()
	{		
		outputs.push_back(new AmplOutputScalar<int>(true));
		outputs.push_back(new AmplOutputScalar<float>(true));
		outputs.push_back(new AmplOutputVector<int, int>());
		outputs.push_back(new AmplOutputVector<int, float>());
		outputs.push_back(new AmplOutputSet<int>());
		outputs.push_back(new AmplOutputSet<float>());
		outputs.push_back(new AmplOutputMatrix<int, int, int>());
		outputs.push_back(new AmplOutputMatrix<int, int, float>());
	}
}
