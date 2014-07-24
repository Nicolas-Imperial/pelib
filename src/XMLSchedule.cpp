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
XMLSchedule::dump(ostream& os, const StreamingAppData &data) const
{
	dump(os, &data);
}

void
XMLSchedule::dump(ostream& os, const StreamingAppData *data) const
{
	const Schedule *sched = dynamic_cast<const Schedule* >(data);
	if(sched == NULL) throw CastException("parameter \"data\" was not of type \"Schedule*\".");

	map<int, std::map<float, Task> > schedule = sched->getSchedule();
	float target_makespan = sched->getRoundTime();

	set<string> task_ids;
	
	for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.getTaskId());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<schedule name=\"" << sched->getName() << "\" autname=\"" << sched->getAUTName() << "\" "
		<< "roundtime=\"" << target_makespan << "\" "
		<< "cores=\"" << schedule.size() << "\" "
		<< "tasks=\"" << task_ids.size() << "\""
		<< "> \n";

	for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p - 1 << "\">\n";
		std::map<float, Task> core_schedule = i->second;
		int order = 0;

		for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			string taskid = j->second.getTaskId();
			os << "   <task taskid=\"" << taskid << "\" "
				<< "starting_time=\"" << j->second.getStartTime() << "\" "
				<< "frequency=\"" << j->second.getFrequency() << "\" "
				<< "width=\"" << j->second.getWidth() << "\" "
				<< "workload=\"" << j->second.getWorkload() << "\" "
				<< "/>\n";
		}
		os << " </core>\n";
	}

	os << "</schedule>\n";
}

Schedule*
XMLSchedule::parse(istream &is) const
{
	DomParser* theSchedule = new DomParser();
	//theSchedule->set_throw_messages(false);
	//igraph_set_error_handler(igraph_error_handler_t* new_handler);
	
	try
	{
		theSchedule->parse_stream(is);
	} catch(xmlpp::parse_error &e)
	{
		throw ParseException(std::string("xmlpp::parse_error"));
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

	try
	{
		float M = stof(root->get_attribute_value("roundtime"));
		std::string name = root->get_attribute_value("name");
		std::string aut_name = root->get_attribute_value("autname");

		Schedule *sched = new Schedule(name, aut_name);
		sched->setRoundTime(M);
		std::map<int, std::map<float, Task> > schedule;
	
		for(Node::NodeList::iterator iter = processors.begin(); iter != processors.end(); ++iter) //for each core
		{
			if((*iter)->get_name().compare("core")) //skip indentation characters et cetera
			{
				continue;
			}

			int core_id = stoi(dynamic_cast<Element*>(*iter)->get_attribute_value("coreid"));
			std::map<float, Task> core_schedule_map;

			auto tasks = (*iter)->get_children();
			int task_id = 1;
			for(auto taskiter = tasks.begin(); taskiter != tasks.end(); ++taskiter, task_id++)
			{
				if((*taskiter)->get_name().compare("task")) //skip indentation characters et cetera
				{
					continue;
				}

				auto *igraph_task = dynamic_cast<Element*>(*taskiter);

				Task task(task_id, igraph_task->get_attribute_value("taskid"));
				task.setFrequency(stof(igraph_task->get_attribute_value("frequency")));
				task.setWidth(stof(igraph_task->get_attribute_value("width")));
				task.setWorkload(stof(igraph_task->get_attribute_value("workload")));
				task.setStartTime(stof(igraph_task->get_attribute_value("starting_time")));
				core_schedule_map.insert(std::pair<float, Task>(task.getStartTime(), task)); 
			}

			schedule.insert(std::pair<int, std::map<float, Task> >(core_id, core_schedule_map));
		}
		
		sched->setSchedule(schedule);

		return sched;
	}
	catch(std::invalid_argument &e)
	{
		throw ParseException(std::string("std::invalid_argument"));
	}
}

XMLSchedule*
XMLSchedule::clone() const
{
	return new XMLSchedule();
}
