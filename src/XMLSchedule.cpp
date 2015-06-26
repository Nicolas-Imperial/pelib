#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

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

XMLSchedule::~XMLSchedule()
{
	// Do nothing
}

void
XMLSchedule::dump(ostream& os, const Record &data) const
{
	dump(os, &data);
}

void
XMLSchedule::dump(ostream& os, const Record *data) const
{
	const Schedule *sched = dynamic_cast<const Schedule* >(data);
	if(sched == NULL) throw CastException("parameter \"data\" was not of type \"Schedule*\".");

	Schedule::table schedule = sched->getSchedule();
	float target_makespan = sched->getRoundTime();

	set<string> task_ids;
	
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.first->getTaskId());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<schedule name=\"" << sched->getName() << "\" autname=\"" << sched->getName() << "\" "
		<< "roundtime=\"" << target_makespan << "\" "
		<< "cores=\"" << schedule.size() << "\" "
		<< "tasks=\"" << task_ids.size() << "\""
		<< "> \n";

	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p - 1 << "\">\n";
		Schedule::sequence core_schedule = i->second;
		int order = 0;

		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			string taskid = j->second.first->getTaskId();
			os << "   <task taskid=\"" << taskid << "\" "
				<< "starting_time=\"" << j->second.first->getStartTime() << "\" "
				<< "frequency=\"" << j->second.first->getFrequency() << "\" "
				<< "width=\"" << j->second.first->getWidth() << "\" "
				<< "workload=\"" << j->second.first->getWorkload() << "\" "
				<< "/>\n";
		}
		os << " </core>\n";
	}

	os << "</schedule>\n";
}

Schedule*
XMLSchedule::parse(istream &is, Taskgraph &taskgraph) const
{
	DomParser* theSchedule = new DomParser();
	//theSchedule->set_throw_messages(false);
	//igraph_set_error_handler(igraph_error_handler_t* new_handler);
	Taskgraph tg = taskgraph;
	
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
	xmlpp::Node::NodeList processors = root->get_children();

	try
	{
		float M = atof(root->get_attribute_value("roundtime").c_str());
		std::string name = root->get_attribute_value("name");
		std::string aut_name = root->get_attribute_value("autname");

		Schedule::table schedule;
		for(xmlpp::Node::NodeList::iterator iter = processors.begin(); iter != processors.end(); ++iter) //for each core
		{
			if((*iter)->get_name().compare("core")) //skip indentation characters et cetera
			{
				continue;
			}

			int core_id = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("coreid").c_str());
			Schedule::sequence core_schedule_map;

			std::list<xmlpp::Node*> itasks = (*iter)->get_children();
			for(std::list<xmlpp::Node*>::iterator taskiter = itasks.begin(); taskiter != itasks.end(); ++taskiter)
			{
				if((*taskiter)->get_name().compare("task")) //skip indentation characters et cetera
				{
					continue;
				}

				Element *igraph_task = dynamic_cast<Element*>(*taskiter);

				Task &task = (Task&) tg.findTask(igraph_task->get_attribute_value("taskid"));
				//Task task(igraph_task->get_attribute_value("taskid"));
				//cout << "Counter = " << task_id << ", tg_task.getId() = " << tg_task.getId() << ", tg_task.getTaskId() = \"" << tg_task.getTaskId() << "." << endl; 

				//Task task(tg_task.getId(), tg_task.getTaskId());
				//Task& task_tg = (Task&)*tg.getTasks().find(task);

				task.setFrequency(atof(igraph_task->get_attribute_value("frequency").c_str()));
				task.setWidth(atof(igraph_task->get_attribute_value("width").c_str()));
				//task.setWorkload(atof(igraph_task->get_attribute_value("workload").c_str()));
				task.setStartTime(atof(igraph_task->get_attribute_value("starting_time").c_str()));

				core_schedule_map.insert(std::pair<float, Schedule::work>(task.getStartTime(), pair<Task*, double>(&task, atof(igraph_task->get_attribute_value("workload").c_str()))));
			}

			schedule.insert(std::pair<int, Schedule::sequence>(core_id, core_schedule_map));
		}
		
		Schedule *sched = new Schedule(name, aut_name, tg, schedule, M);

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
