#include <ParseException.hpp>
#include <CastException.hpp>

#include <StreamNet.hpp>
#include <AmplArchitecture.hpp>
#include <XMLSchedule.hpp>

namespace pelib
{
	StreamNet::StreamNet()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	StreamNet::~StreamNet()
	{
		deleteParsers();
		deleteOutputs();			
	}
	
	void
	StreamNet::deleteParsers()
	{
		/*
		for(std::vector<TaskgraphParser*>::iterator i = taskgraphParsers.begin(); i != taskgraphParsers.end(); i = taskgraphParsers.erase(i))
		{
			delete *i;
		}
		*/
		for(std::vector<ArchitectureParser*>::iterator i = architectureParsers.begin(); i != architectureParsers.end(); i = architectureParsers.erase(i))
		{
			delete *i;
		}
		for(std::vector<ScheduleParser*>::iterator i = scheduleParsers.begin(); i != scheduleParsers.end(); i = scheduleParsers.erase(i))
		{
			delete *i;
		}
	}

	void
	StreamNet::deleteOutputs()
	{
		for(std::vector<StreamingAppDataOutput*>::iterator i = outputs.begin(); i != outputs.end(); i = outputs.erase(i))
		{
			delete *i;
		}
	}

	StreamNet&
	StreamNet::operator=(const StreamNet &src)
	{
		deleteParsers();
		deleteOutputs();

		/*
		for(std::vector<TaskgraphParser*>::iterator i = taskgraphParsers.begin(); i != taskgraphParsers.end(); i = taskgraphParsers.erase(i))
		{
			taskgraphParsers.push_back((*i)->clone());
		}
		*/
		
		for(std::vector<ArchitectureParser*>::iterator i = architectureParsers.begin(); i != architectureParsers.end(); i = architectureParsers.erase(i))
		{
			architectureParsers.push_back((*i)->clone());
		}
		
		for(std::vector<ScheduleParser*>::iterator i = scheduleParsers.begin(); i != scheduleParsers.end(); i = scheduleParsers.erase(i))
		{
			scheduleParsers.push_back((*i)->clone());
		}
		
		for(std::vector<StreamingAppDataOutput*>::const_iterator i = src.outputs.begin(); i != src.outputs.end(); i++)
		{
			outputs.push_back((*i)->clone());
		}

		return *this;
	}

	StreamingApp
	StreamNet::parse(std::istream &taskgraph, std::istream &architecture, std::istream &schedule) const
	{
		StreamingApp record;
		std::istream *inputs[3] = {&taskgraph, &architecture, &schedule};

		for(int i = 0; i < 3; i++)
		{
			// Read the whole input once
			std::string line;
			std::stringstream ss;
			while(!getline(*inputs[i], line).fail())
			{
				ss << line << std::endl;
			}
			line = ss.str();

			/*
			for(std::vector<TaskgraphParser*>::const_iterator iter = this->taskgraphParsers.begin(); iter != this->taskgraphParsers.end(); iter++)
			{
				std::stringstream input;
				input.str(line);
				
				TaskgraphParser *parser = *iter;
				try {
				Taskgraph *data = parser->parse(input);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
				}
			}
		*/
			for(std::vector<ArchitectureParser*>::const_iterator iter = this->architectureParsers.begin(); iter != this->architectureParsers.end(); iter++)
			{				
				std::stringstream input;
				input.str(line);
				
				ArchitectureParser *parser = *iter;
				try {
					Architecture *data = parser->parse(input);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
				}
			}
			
			for(std::vector<ScheduleParser*>::const_iterator iter = this->scheduleParsers.begin(); iter != this->scheduleParsers.end(); iter++)
			{
				std::stringstream input;
				input.str(line);
				
				ScheduleParser *parser = *iter;
				try {
					Schedule *data = parser->parse(input);
					record.insert(data);

					// No need to try another parser; proceed with the next token
					break;
				} catch (ParseException &e)
				{
				}
			}
		}
				
		return record;
	}

	void
	StreamNet::dump(std::ostream& o, const StreamingAppData &data) const
	{
		dump(o, &data);
	}

	void
	StreamNet::dump(std::ostream& o, const StreamingAppData *data) const
	{
		for (std::vector<StreamingAppDataOutput*>::const_iterator out = outputs.begin(); out != outputs.end(); out++)
		{
			const StreamingAppDataOutput *output = *out;
			
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
	StreamNet::addParsers()
	{
		architectureParsers.push_back(new AmplArchitecture());
		scheduleParsers.push_back(new XMLSchedule());
	}

	void			
	StreamNet::addOutputs()
	{		
		// Add outputs here
		outputs.push_back(new AmplArchitecture());
		outputs.push_back(new XMLSchedule());
	}
}
