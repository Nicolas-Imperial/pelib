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

#include <pelib/AmplInput.hpp>
#include <pelib/Algebra.hpp>

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

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
	const set<Link> &links = sched->getLinks();
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
		<< "<schedule scheduler=\"" << sched->getName() << "\" application=\"" << sched->getAppName() << "\""
		//<< "makespan=\"" << target_makespan << "\" "
		<< ">" << endl;

	// Finds and set the precision required for this schedule
	std::streamsize old_precision = os.precision();
	std::streamsize max_work_precision = 0;
	std::streamsize max_freq_precision = 0;
	std::streamsize max_start_precision = 0;
	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		float last_start = 0;
		float last_time = 0;
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			Task t = *j->second.first;
			float start_time = j->first;
			if(start_time > 0)
			{
				//float delta = t.getStartTime() - last_start;
				float delta = start_time - last_start;
				delta = delta - floor(delta);
				std::streamsize precision = (std::streamsize)(ceil(-log10(delta)) + 2);
				if(precision > max_start_precision)
				{
					max_start_precision = precision;
				}
				//last_start = t.getStartTime();
				last_start = start_time;

				//delta = abs(t.getStartTime() - (last_start + last_time));
				delta = abs(start_time - (last_start + last_time));
				delta = delta - floor(delta);
				precision = (std::streamsize)(ceil(-log10(delta)) + 1);
				if(precision > max_start_precision)
				{
					max_start_precision = precision;
				}
				const Task &tgt = *tg->getTasks().find(t);
				last_time = tgt.runtime(t.getWidth(), t.getFrequency());

				double work = t.getWorkload() - floor(t.getWorkload());
				precision = (std::streamsize)(ceil(-log10(work)) + 1);
				if(precision > max_work_precision)
				{
					max_work_precision = precision;
				}

				double freq = t.getFrequency() - floor(t.getFrequency());
				precision = (std::streamsize)(ceil(-log10(freq)) + 1);
				if(precision > max_freq_precision)
				{
					max_freq_precision = precision;
				}

				/*
				os << "<!-- core : " << i->first << " -->" << endl;
				os << "<!-- task : " << t.getName() << " -->" << endl;
				os << "<!-- start_time = " << t.getStartTime() << " -->" << endl;
				os << "<!-- last_start = " << last_start << " -->" << endl;
				*/

				// update last time
			}
		}
	}

	for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
	{
		int p = i->first;
		os << " <core coreid=\"" << p + 1 << "\">" << endl;
		Schedule::sequence core_schedule = i->second;
		int order = 0;

		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, order++)
		{
			Task t = *j->second.first;
			float start = j->first;
			string taskid = t.getName();
			size_t task_index = std::distance(sched->getUniqueTasks().begin(), sched->getUniqueTasks().find(t));
			
			os << "  <task name=\"" << taskid << "\" ";
			os << setprecision(max_start_precision);
			//os << "start=\"" << std::fixed << (t.getStartTime() > 0 ? t.getStartTime() : start) << "\" ";
			os << "start=\"" << std::fixed << start << "\" ";
			os << setprecision(old_precision);
			os << "frequency=\"" << (float)t.getFrequency() << "\" ";
			os << "width=\"" << t.getWidth() << "\" ";
			os << "workload=\"" << t.getWorkload() << "\"";
			os << "/>" << endl;

			set<Task>::iterator iter = tasks.begin();
			std::advance(iter, task_index);
			t.setMaxWidth(iter->getMaxWidth());
			t.setEfficiencyString(string(iter->getEfficiencyString()));
			string eff = string(iter->getEfficiencyString());
			start += t.runtime(t.getWidth(), t.getFrequency());
		}
		os << " </core>" << endl;
	}

	for(set<Link>::iterator i = links.begin(); i != links.end(); i++)
	{
		const Link &link = *i;

		os << " <link>" << endl;
		os << "  <producer ";
		os << "name=\"" << link.getProducerName() << "\" ";
		os << "task=\"" << link.getProducer()->getName() << "\" ";
		os << "core=\"" << link.getProducerBuffer().getCore() + 1 << "\" ";
		os << "memory=\"" << Buffer::memoryTypeToString(link.getProducerBuffer().getMemoryType()) << "\" ";
		os << "level=\"" << link.getProducerBuffer().getLevel() << "\"";
		os << "/>" << endl;

		os << "  <consumer ";
		os << "name=\"" << link.getConsumerName() << "\" ";
		os << "task=\"" << link.getConsumer()->getName() << "\"";
		os << "core=\"" << link.getConsumerBuffer().getCore() + 1 << "\" ";
		os << "memory=\"" << Buffer::memoryTypeToString(link.getConsumerBuffer().getMemoryType()) << "\" ";
		os << "level=\"" << link.getConsumerBuffer().getLevel() << "\"";
		os << "/>" << endl;

		os << "  <buffer type=\"" << link.getQueueBuffer().getType() << "\" ";
		if(link.getQueueBuffer().getHeader().compare(string()) != 0)
		{
			os << "header=\"" << link.getQueueBuffer().getHeader() << "\" ";
		}
		os << "size=\"" << link.getQueueBuffer().getSize() << "\" ";
		os << "core=\"" << link.getQueueBuffer().getCore() + 1 << "\" ";
		os << "memory=\"" << Buffer::memoryTypeToString(link.getQueueBuffer().getMemoryType()) << "\" ";
		os << "level=\"" << link.getQueueBuffer().getLevel() << "\"";
		os << "/>" << endl;

		os << " </link>" << endl;

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
		std::string name = root->get_attribute_value("scheduler");
		std::string aut_name = root->get_attribute_value("application");

		Schedule::table schedule;
		set<Task> tasks;
		set<Link> links;
		for(xmlpp::Node::NodeList::iterator iter = processors.begin(); iter != processors.end(); ++iter) //for each core
		{
			if((*iter)->get_name().compare("core") == 0) //skip indentation characters et cetera
			{
				int core_id = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("id").c_str()) - 1;
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
					task.setWorkload(atof(igraph_task->get_attribute_value("workload").c_str()));
					//task.setStartTime(atof(igraph_task->get_attribute_value("start").c_str()));
					float start_time = atof(igraph_task->get_attribute_value("start").c_str());
					tasks.insert(task);
					const Task &t = *tasks.find(task);

					core_schedule_map.insert(std::pair<float, Schedule::work>(start_time, pair<const Task*, double>(&t, atof(igraph_task->get_attribute_value("workload").c_str()))));
				}

				schedule.insert(std::pair<int, Schedule::sequence>(core_id, core_schedule_map));
			}
			else if((*iter)->get_name().compare("link") == 0) //skip indentation characters et cetera
			{
				/*
				string type = dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("type").c_str();
				string header = dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("header").c_str();
				size_t size = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("size").c_str());
				unsigned int core = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("core").c_str());
				Buffer::MemoryType memory = Buffer::stringToMemoryType(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("memory").c_str());
				unsigned int level = atoi(dynamic_cast<xmlpp::Element*>(*iter)->get_attribute_value("level").c_str());
				*/

				string queueType, queueHeader;
				size_t queueSize;
				unsigned int queueCore;
				Buffer::MemoryType queueMemory;
				unsigned int queueLevel;
				string producer_name, consumer_name;
				unsigned int producer_level, consumer_level;
				unsigned int producer_core, consumer_core;
				Buffer::MemoryType producer_type, consumer_type;
				const Task *producer = NULL, *consumer = NULL;

				std::list<xmlpp::Node*> itasks = (*iter)->get_children();
				for(std::list<xmlpp::Node*>::iterator taskiter = itasks.begin(); taskiter != itasks.end(); ++taskiter)
				{
					if((*taskiter)->get_name().compare("producer") == 0)
					{
						Element *igraph_task = dynamic_cast<Element*>(*taskiter);
						producer_name = igraph_task->get_attribute_value("name");
						string task_name = igraph_task->get_attribute_value("task");
						producer_level = atoi(igraph_task->get_attribute_value("level").c_str());
						producer_core = atoi(igraph_task->get_attribute_value("core").c_str()) - 1;
						producer_type = Buffer::stringToMemoryType(igraph_task->get_attribute_value("memory").c_str());
						set<Task>::iterator i;
						for(i = tasks.begin(); i != tasks.end(); i++)
						{
							if(i->getName().compare(task_name) == 0)
							{
								producer = &*i;
								break;
							}
						}
						if(i == tasks.end())
						{
							throw PelibException("Could not find producer task in task set");
						}
					}
					else if((*taskiter)->get_name().compare("consumer") == 0)
					{
						Element *igraph_task = dynamic_cast<Element*>(*taskiter);
						consumer_name = igraph_task->get_attribute_value("name");
						string task_name = igraph_task->get_attribute_value("task");
						consumer_level = atoi(igraph_task->get_attribute_value("level").c_str());
						consumer_core = atoi(igraph_task->get_attribute_value("core").c_str()) - 1;
						consumer_type = Buffer::stringToMemoryType(igraph_task->get_attribute_value("memory").c_str());
						set<Task>::iterator i;
						for(i = tasks.begin(); i != tasks.end(); i++)
						{
							if(i->getName().compare(task_name) == 0)
							{
								consumer = &*i;
								break;
							}
						}
						if(i == tasks.end())
						{
							throw PelibException("Could not find consumer task in task set");
						}
					}
					else if((*taskiter)->get_name().compare("buffer") == 0)
					{

						Element *iter = dynamic_cast<Element*>(*taskiter);
						queueType = iter->get_attribute_value("type").c_str();
						queueHeader = iter->get_attribute_value("header").c_str();
						queueSize = atoi(iter->get_attribute_value("size").c_str());
						queueCore = atoi(iter->get_attribute_value("core").c_str()) - 1;
						queueMemory = Buffer::stringToMemoryType(iter->get_attribute_value("memory").c_str());
						queueLevel = atoi(iter->get_attribute_value("level").c_str());
					}
				}

				if(producer == NULL || consumer == NULL)
				{
					throw PelibException("Missing either reference to producer or consumer task in link");
				}

				Link link(*producer, *consumer, producer_name, consumer_name, Buffer(queueSize, queueType, queueMemory, queueLevel, queueCore), Buffer(0, "", producer_type, producer_level, producer_core), Buffer(0, "", consumer_type, consumer_level, consumer_core));
				links.insert(link);
			}
		}
		
		Schedule *sched = new Schedule(name, aut_name, schedule, tasks, links);

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
