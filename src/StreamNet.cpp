#include <ParseException.hpp>
#include <CastException.hpp>

#include <StreamNet.hpp>
#include <GraphML.hpp>
#include <AmplPlatform.hpp>
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

		for(std::vector<PlatformParser*>::iterator i = platformParsers.begin(); i != platformParsers.end(); i = platformParsers.erase(i))
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
		
		for(std::vector<PlatformParser*>::iterator i = platformParsers.begin(); i != platformParsers.end(); i = platformParsers.erase(i))
		{
			platformParsers.push_back((*i)->clone());
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
	StreamNet::parse(std::istream &taskgraph, std::istream &platform, std::istream &schedule) const
	{
		StreamingApp record;
		std::string tg_str, arch_str, sched_str;
		std::stringstream ss;

		// Taskgraph, Platform and Schedule data structures
		Taskgraph *tg = NULL;
		Platform *arch;
		Schedule *sched;

		// Read the whole inputs once
		// Taskgraph
		ss.str(std::string());
		while(!getline(taskgraph, tg_str).fail())
		{
			ss << tg_str << std::endl;
		}
		tg_str = ss.str();

		// Platform
		ss.str(std::string());
		while(!getline(platform, arch_str).fail())
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
				tg = parser->parse(input);
				record.insert(tg);

				// No need to try another parser; proceed with the next token
				break;
			} catch (ParseException &e)
			{
			}
		}

		// Platform
		for(std::vector<PlatformParser*>::const_iterator iter = this->platformParsers.begin(); iter != this->platformParsers.end(); iter++)
		{				
			std::stringstream input;
			input.str(arch_str);
			
			PlatformParser *parser = *iter;
			try {
				arch = parser->parse(input);
				record.insert(arch);

				// No need to try another parser; proceed with the next token
				break;
			} catch (ParseException &e)
			{
			}
		}

		// Schedule
		for(std::vector<ScheduleParser*>::const_iterator iter = this->scheduleParsers.begin(); iter != this->scheduleParsers.end() && tg != NULL; iter++)
		{
			std::stringstream input;
			input.str(sched_str);
				
			ScheduleParser *parser = *iter;
			try {
				sched = parser->parse(input, *tg);
				record.insert(sched);

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
		
		platformParsers.push_back(new AmplPlatform());
		
		scheduleParsers.push_back(new XMLSchedule());
	}

	void			
	StreamNet::addOutputs()
	{		
		// Add outputs here
		outputs.push_back(new GraphML());
		outputs.push_back(new AmplPlatform());
		outputs.push_back(new XMLSchedule());
	}
}
