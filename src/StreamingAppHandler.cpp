#include <ParseException.hpp>
#include <CastException.hpp>

#include <StreamingAppHandler.hpp>

namespace pelib
{
	StreamingAppHandler::StreamingAppHandler()
	{
		// Add parsers
		addParsers();
				
		// Add interpreters
		addOutputs();
	}

	StreamingAppHandler::StreamingAppHandler(std::vector<ScheduleParser*> scheduleParsers,
			std::vector<ScheduleOutput*> scheduleOutputs)
	{
		this->scheduleParsers = scheduleParsers;
		this->scheduleOutputs = scheduleOutputs;
	}

	StreamingAppHandler::~StreamingAppHandler()
	{
		deleteParsers();
		deleteOutputs();			
	}
	
	void
	StreamingAppHandler::deleteParsers()
	{
		
		for(std::vector<ScheduleParser*>::iterator i = scheduleParsers.begin(); i != scheduleParsers.end(); i = scheduleParsers.erase(i))
		{
			delete *i;
		}
	}

	void
	StreamingAppHandler::deleteOutputs()
	{
		for(std::vector<ScheduleOutput*>::iterator i = scheduleOutputs.begin(); i != scheduleOutputs.end(); i = scheduleOutputs.erase(i))
		{
			delete *i;
		}
	}

	StreamingAppHandler&
	StreamingAppHandler::operator=(const StreamingAppHandler &src)
	{
		deleteParsers();
		deleteOutputs();

		for(std::vector<ScheduleParser*>::const_iterator i = src.scheduleParsers.begin(); i != src.scheduleParsers.end(); i++)
		{
			scheduleParsers.push_back((*i)->clone());
		}

		for(std::vector<ScheduleOutput*>::const_iterator i = src.scheduleOutputs.begin(); i != src.scheduleOutputs.end(); i++)
		{
			scheduleOutputs.push_back((*i)->clone());
		}

		return *this;
	}

	StreamingApp
	StreamingAppHandler::parse(std::istream &ampl_data)
	{
		// Do something
		return StreamingApp();
	}

	void
	StreamingAppHandler::dump(std::ostream& o, const StreamingApp &record) const
	{
		// Do something
	}

	void
	StreamingAppHandler::dumpSchedule(std::ostream& o, const Schedule *data) const
	{
		for (std::vector<ScheduleOutput*>::const_iterator out = scheduleOutputs.begin(); out != scheduleOutputs.end(); out++)
		{
			const ScheduleOutput *output = *out;
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
	StreamingAppHandler::addParsers()
	{
		// Add parsers here
	}

	void			
	StreamingAppHandler::addOutputs()
	{		
		// Add outputs here
	}
}
