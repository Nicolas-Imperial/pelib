/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

extern "C"{
#include <igraph.h>
}

#include <pelib/Schedule.hpp>
#include <pelib/XMLSchedule.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/Task.hpp>
#include <pelib/Taskgraph.hpp>

#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>

#ifndef debug
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#endif

using namespace pelib;
using namespace std;
using namespace xmlpp;

XMLSchedule::~XMLSchedule()
{
	// Do nothing
}

void
XMLSchedule::dump(ostream& os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const
{
	dump(os, &data, &tg, &pt);
}

void
XMLSchedule::dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt) const
{
	Schedule::table schedule = sched->getSchedule();
	//float target_makespan = sched->getMakespan();
	set<Task> &tasks = (set<Task>&)tg->getTasks();
	set<string> task_ids;
	
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			task_ids.insert(j->second.first->getName());
		}
	}

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
		<< "<schedule name=\"" << sched->getName() << "\" appname=\"" << sched->getAppName() << "\""
		//<< "makespan=\"" << target_makespan << "\" "
		<< ">" << endl;

	// Finds and set the precision required for this schedule
	std::streamsize old_precision = os.precision();
	float min_delta = FLT_MAX;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		float last_start = 0;
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task t = *j->second.first;
			if(t.getStartTime() > 0)
			{
				float delta = t.getStartTime() - last_start;
				if(delta < min_delta)
				{
					min_delta = delta;
				}
				/*
				os << "<!-- core : " << i->first << " -->" << endl;
				os << "<!-- task : " << t.getName() << " -->" << endl;
				os << "<!-- start_time = " << t.getStartTime() << " -->" << endl;
				os << "<!-- last_start = " << last_start << " -->" << endl;
				*/

				// update last time
				last_start = t.getStartTime();
			}
		}
	}
	std::streamsize precision = (std::streamsize)(ceil(-log10(min_delta)) + 1);
	//os << "<!-- precision = " << precision << " -->" << endl;

	os << setprecision(precision);
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p << "\">" << endl;
		Schedule::sequence core_schedule = i->second;
		int order = 0;
		double start = 0;

		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			string taskid = j->second.first->getName();
			Task t = *j->second.first;
			size_t task_index = std::distance(sched->getTasks().begin(), sched->getTasks().find(t));
			
			os << "  <task name=\"" << taskid << "\" "
				<< "start=\"" << (t.getStartTime() > 0 ? t.getStartTime() : start) << "\" "
				<< "frequency=\"" << t.getFrequency() << "\" "
				<< "width=\"" << t.getWidth() << "\" "
				<< "workload=\"" << t.getWorkload() << "\""
				<< "/>" << endl;

				set<Task>::iterator iter = tasks.begin();
				std::advance(iter, task_index);
				t.setMaxWidth(iter->getMaxWidth());
				t.setEfficiencyString(iter->getEfficiencyString());
				start += t.runtime(t.getWidth(), t.getFrequency());
		}
		os << " </core>" << endl;
	}
	os << setprecision(old_precision);

	os << "</schedule>" << endl;
}

Schedule*
XMLSchedule::parse(istream &is) const
{
	DomParser* theSchedule = new DomParser();
	//theSchedule->set_throw_messages(false);
	//igraph_set_error_handler(igraph_error_handler_t* new_handler);
	//Taskgraph tg = taskgraph;
	
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
		std::string name = root->get_attribute_value("name");
		std::string aut_name = root->get_attribute_value("appname");
		set<Task> tasks;

		Schedule::table schedule;
		for(xmlpp::Node::NodeList::iterator iter = processors.begin(); iter != processors.end(); ++iter) //for each core
		{
			if((*iter)->get_name().compare("core")) //skip indentation characters et cetera
			{
				continue;
			}

			int core_id = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("coreid").c_str());
			Schedule::sequence core_schedule_map;
			//cout << "Core " << core_id << endl;

			std::list<xmlpp::Node*> itasks = (*iter)->get_children();
			for(std::list<xmlpp::Node*>::iterator taskiter = itasks.begin(); taskiter != itasks.end(); ++taskiter)
			{
				if((*taskiter)->get_name().compare("task") != 0) //skip indentation characters et cetera
				{
					//cout << "Skipping " << (*taskiter)->get_name() << endl;
					continue;
				}

				Element *igraph_task = dynamic_cast<Element*>(*taskiter);
				//cout << "Task " << igraph_task->get_attribute_value("name") << endl;

				//Task &task = (Task&) tg.findTask(igraph_task->get_attribute_value("taskid"));
				Task task(igraph_task->get_attribute_value("name"));
				//cout << "Counter = " << task_id << ", tg_task.getName() = " << tg_task.getName() << ", tg_task.getTaskId() = \"" << tg_task.getTaskId() << "." << endl; 

				//Task task(tg_task.getName(), tg_task.getTaskId());
				//Task& task_tg = (Task&)*tg.getTasks().find(task);

				task.setFrequency(atof(igraph_task->get_attribute_value("frequency").c_str()));
				task.setWidth(atof(igraph_task->get_attribute_value("width").c_str()));
				//task.setWorkload(atof(igraph_task->get_attribute_value("workload").c_str()));
				task.setStartTime(atof(igraph_task->get_attribute_value("start").c_str()));
				tasks.insert(task);
				const Task &t = *tasks.find(task);

				core_schedule_map.insert(std::pair<float, Schedule::work>(task.getStartTime(), pair<const Task*, double>(&t, atof(igraph_task->get_attribute_value("workload").c_str()))));
			}

			schedule.insert(std::pair<int, Schedule::sequence>(core_id, core_schedule_map));
		}
		
		Schedule *sched = new Schedule(name, aut_name, schedule);

		/*
		for(set<const Task*>::const_iterator j = sched->getTasks(1).begin(); j != sched->getTasks(1).end(); j++)
		{
			const Task *jj = *j;
			debug(jj);
			Task jjj = *jj;
			debug(jjj.getName());
		}
		debug("Out from XML Schedule parser");
		*/

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
