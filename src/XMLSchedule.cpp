#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>

extern "C"{
#include <igraph.h>
}

#include "Schedule.hpp"
#include "XMLSchedule.hpp"

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <Task.hpp>

#include <CastException.hpp>
#include <ParseException.hpp>

using namespace pelib;
using namespace std;
using namespace xmlpp;

void
XMLSchedule::dump(ostream& os, const StreamingAppData *data) const
{
	const Schedule *sched = dynamic_cast<const Schedule* >(data);
	if(sched == NULL) throw CastException("parameter \"data\" was not of type \"Schedule*\".");
	//sr.theSchedule->get_document()->write_to_stream(os);

	map<int, std::vector<Task> > schedule = sched->getSchedule();
	int processors = schedule.size();
	float target_makespan = sched->getRoundTime();

	set<string> task_ids;
	for(map<int, vector<Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(vector<Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->getId());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<schedule name=\"schedule\" autname=\"" << sched->getAUTName() << "\" "
		<< "cores=\"" << processors << "\" "
		<< "tasks=\"" << task_ids.size() << "\" "
		<< "roundtime=\"" << target_makespan << "\">\n";

	for(map<int, vector<Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p << "\">\n";
		std::vector<Task> core_schedule = i->second;
		int order = 0;

		for(vector<Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			//int id = j->getId(); // fixme: change string ID to number ID
			//maxid = max(maxid, id);
			string taskid = j->getId();

			/*
			if(id > 0)
			{ 
				// Bounds check. Will throw if there are more (higher numbered) tasks in amploutput schedule than in taskgraphrecord.
				try
				{
					//taskid = taskids[id-1];
					taskid = taskids.at(id-1); // With bounds check
				}
				catch (std::out_of_range e)
				{
					cerr << "Error: Missmatch, ampl_output task id " << id << " does not exist in the task graph\n";
					throw e;
				}

				if(id == 0) // 0 = no more tasks scheduled on this core
				{
					break;
				}
				*/
				os << "   <task taskid=\"" << taskid << "\" ordering=\"" << order << "\" "
					<< "frequency=\"" << j->getFrequency() <<  "\" />\n";
			//}
		}
		os << " </core>\n";
	}
	os << "</schedule>\n";
}

Schedule*
XMLSchedule::parse(istream &is)
{
	DomParser* theSchedule = new DomParser();
	//theSchedule->set_throw_messages(false);
	//igraph_set_error_handler(igraph_error_handler_t* new_handler);
	
	try
	{
		theSchedule->parse_stream(is);
	} catch(xmlpp::parse_error &e)
	{
		throw ParseException(std::string("Not a valid XML format"));
	}
	
	// Just like taskgraph conversion
	// this involves turning the data structure
	// inside out, and to convert from
	// the taskids to id 1..n, in a 
	// deterministic fashion.
	// The latter is slightly more complex
	// than taskgraph version
	// since the tasks may appear more than one
	// in the schedule.

	Element *root = theSchedule->get_document()->get_root_node();
	Node::NodeList processors = root->get_children();

	float M = stof(root->get_attribute_value("roundtime"));
	std::string name = root->get_attribute_value("name");
	std::string aut_name = root->get_attribute_value("autname");

	Schedule *sched = new Schedule(name, aut_name);
	sched->setRoundTime(M);
	std::map<int, std::vector<Task> > schedule;
	
	for(Node::NodeList::iterator iter = processors.begin(); iter != processors.end(); ++iter) //for each core
	{
		if((*iter)->get_name().compare("core")) //skip indentation characters et cetera
		{
			continue;
		}

		int core_id = stoi(dynamic_cast<Element*>(*iter)->get_attribute_value("coreid"));
		std::vector<Task> core_schedule;
		std::map<int, Task> core_schedule_map;

		auto tasks = (*iter)->get_children();
		for(auto taskiter = tasks.begin(); taskiter != tasks.end(); ++taskiter)
		{
			if((*taskiter)->get_name().compare("task")) //skip indentation characters et cetera
			{
				continue;
			}

			auto *igraph_task = dynamic_cast<Element*>(*taskiter);

			Task task(igraph_task->get_attribute_value("taskid"));
			task.setFrequency(stoi(igraph_task->get_attribute_value("frequency")));
			core_schedule_map.insert(std::pair<int, Task>(stoi(igraph_task->get_attribute_value("ordering")), task)); 
		}

		for(auto taskiter = core_schedule_map.begin(); taskiter != core_schedule_map.end(); taskiter++)
		{
			core_schedule.push_back(taskiter->second);
		}

		schedule.insert(std::pair<int, std::vector<Task> >(core_id, core_schedule));
	}

	sched->setSchedule(schedule);

	return sched;
}

XMLSchedule*
XMLSchedule::clone() const
{
	return new XMLSchedule();
}
