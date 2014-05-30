#include <ParseException.hpp>
#include <CastException.hpp>

#include <StreamNet.hpp>
#include <GraphML.hpp>
#include <AmplArchitecture.hpp>
#include <XMLSchedule.hpp>

using namespace std;

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
		for(std::vector<TaskgraphParser*>::iterator i = taskgraphParsers.begin(); i != taskgraphParsers.end(); i = taskgraphParsers.erase(i))
		{
			delete *i;
		}

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

		for(std::vector<TaskgraphParser*>::iterator i = taskgraphParsers.begin(); i != taskgraphParsers.end(); i = taskgraphParsers.erase(i))
		{
			taskgraphParsers.push_back((*i)->clone());
		}
		
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
		std::string tg_str, arch_str, sched_str;
		std::stringstream ss;

		// Read the whole inputs once
		// Taskgraph
		ss.str(std::string());
		while(!getline(taskgraph, tg_str).fail())
		{
			ss << tg_str << std::endl;
		}
		tg_str = ss.str();

		// Architecture
		ss.str(std::string());
		while(!getline(architecture, arch_str).fail())
		{
			ss << arch_str << std::endl;
		}
		arch_str = ss.str();

		// Schedule
		ss.str(std::string());
		while(!getline(schedule, sched_str).fail())
		{
			ss << sched_str << std::endl;
		}
		sched_str = ss.str();

		// Now parse them with whatever we have
		// Taskgraph
		for(std::vector<TaskgraphParser*>::const_iterator iter = this->taskgraphParsers.begin(); iter != this->taskgraphParsers.end(); iter++)
		{
			std::stringstream input;
			input.str(tg_str);
				
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

		// Architecture
		for(std::vector<ArchitectureParser*>::const_iterator iter = this->architectureParsers.begin(); iter != this->architectureParsers.end(); iter++)
		{				
			std::stringstream input;
			input.str(arch_str);
			
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

		// Schedule
		for(std::vector<ScheduleParser*>::const_iterator iter = this->scheduleParsers.begin(); iter != this->scheduleParsers.end(); iter++)
		{
			std::stringstream input;
			input.str(sched_str);
				
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
		taskgraphParsers.push_back(new GraphML());
		
		architectureParsers.push_back(new AmplArchitecture());
		
		scheduleParsers.push_back(new XMLSchedule());
	}

	void			
	StreamNet::addOutputs()
	{		
		// Add outputs here
		outputs.push_back(new GraphML());
		outputs.push_back(new AmplArchitecture());
		outputs.push_back(new XMLSchedule());
	}
}
