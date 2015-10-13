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
#include <pelib/DrakeCSchedule.hpp>

#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/Task.hpp>

#include <pelib/CastException.hpp>
#include <pelib/ParseException.hpp>

#ifndef debug
#if 0
#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
#else
#define debug(var)
#endif
#endif

using namespace pelib;
using namespace std;

DrakeCSchedule::~DrakeCSchedule()
{
	// Do nothing
}

void
DrakeCSchedule::dump(ostream& os, const Schedule &data, const Taskgraph &tg, const Platform &pt) const
{
	dump(os, &data, &tg, &pt);
}

void
DrakeCSchedule::dump(ostream& os, const Schedule *sched, const Taskgraph *tg, const Platform *pt) const
{
	size_t p = sched->getSchedule().size();
	size_t n = tg->getTasks().size();
	map<int, set<const Task*> > tasks, producers, consumers;
	os << "#include <stdlib.h> " << endl << endl;
	os << "#include <drake/schedule.h> " << endl << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		const Task &t = *i;
		os <<
			"#define TASK_NAME " << t.getName() << endl <<
			"#define TASK_MODULE " << t.getModule() << endl <<
			"#include <drake/node.h>" << endl <<
			"#define DONE_" << t.getName() << " 1" << endl << endl;
	}

	os << 
		"int drake_task_number()" << endl <<
		"{" << endl <<
		"	return " << n << ";" << endl <<
		"}" << endl << endl <<
		"char* drake_task_name(size_t index)" << endl <<
		"{" << endl <<
		"	switch(index - 1)" << endl << "	{" << endl;
	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "		case " << std::distance(tg->getTasks().begin(), i) << ":" << endl << "			return \"" << i->getName() << "\";" << endl << "		break;" << endl;
	}
	os << "		default:" << endl << "			return \"invalid task id\";" << endl << "		break;" << endl <<
		"	}" << endl << "}" << endl << endl <<
		"void drake_schedule_init(drake_schedule_t* schedule)" << endl <<
		"{" << endl <<
		"	schedule->core_number = " << p << ";" << endl << 
		"	schedule->task_number = " << n << ";" << endl <<
		"	schedule->stage_time = " << tg->getDeadline(*pt) << ";" << endl << endl <<
		"	schedule->tasks_in_core = malloc(sizeof(size_t) * schedule->core_number);" << endl;

	os << endl << "	schedule->task_name = malloc(sizeof(size_t*) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->task_name[" << std::distance(tg->getTasks().begin(), i) << "] = \"" << i->getName() << "\";" << endl;
	}
	os << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	schedule->tasks_in_core[" << i->first - 1 << "] = " << sched->getTasks(i->first).size() << ";" << endl;
	}

	os << endl << "	schedule->consumers_in_core = malloc(sizeof(size_t) * schedule->core_number);" << endl; 

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		const set<Platform::island> i1 = pt->getSharedMemoryIslands(i->first);
		const set<int> islands = pt->getSharedMemoryIslands(i1);
		os << "	schedule->consumers_in_core[" << i->first - 1 << "] = " << sched->getRemoteSharedMemoryIslandConsumers(islands, *tg, *pt).size() << ";" << endl;
	}

	os << endl << "	schedule->producers_in_core = malloc(sizeof(size_t) * schedule->core_number);" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		const set<Platform::island> i1 = pt->getSharedMemoryIslands(i->first);
		const set<int> islands = pt->getSharedMemoryIslands(i1);
		os << "	schedule->producers_in_core[" << i->first - 1 << "] = " << sched->getRemoteSharedMemoryIslandProducers(islands, *tg, *pt).size() << ";" << endl;
	}

	os << endl << "	schedule->consumers_in_task = malloc(sizeof(size_t) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->consumers_in_task[" << std::distance(tg->getTasks().begin(), i) << "] = " << i->getConsumers().size() << ";" << endl;
	}

	os << endl << "	schedule->producers_in_task = malloc(sizeof(size_t) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->producers_in_task[" << std::distance(tg->getTasks().begin(), i) << "] = " << i->getProducers().size() << ";" << endl;
	}

	os << endl << "	schedule->remote_consumers_in_task = malloc(sizeof(size_t) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->remote_consumers_in_task[" << std::distance(tg->getTasks().begin(), i) << "] = " << sched->getRemoteSharedMemoryIslandTaskConsumers(*i, *tg, *pt).size() << ";" << endl;
	}

	os << endl << "	schedule->remote_producers_in_task = malloc(sizeof(size_t) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->remote_producers_in_task[" << std::distance(tg->getTasks().begin(), i) << "] = " << sched->getRemoteSharedMemoryIslandTaskProducers(*i, *tg, *pt).size() << ";" << endl;
	}

	os << endl << "	schedule->producers_id = malloc(sizeof(size_t*) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->producers_id[" << std::distance(tg->getTasks().begin(), i) << "] = ";
		if (i->getProducers().size() == 0)
		{
			os << "NULL;" << endl;
		}
		else
		{
			os << "malloc(sizeof(size_t) * " << i->getProducers().size() << ");" << endl;
			size_t counter = 0;
			for(set<const Link*>::const_iterator j = i->getProducers().begin(); j != i->getProducers().end(); j++)
			{
				os << "	schedule->producers_id[" << std::distance(tg->getTasks().begin(), i) << "][" << counter << "] = " << std::distance(tg->getTasks().begin(), tg->getTasks().find(*(*j)->getProducer())) + 1 << ";" << endl;
				counter++;
			}
		}
	}

	os << endl << "	schedule->consumers_id = malloc(sizeof(size_t*) * schedule->task_number);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	schedule->consumers_id[" << std::distance(tg->getTasks().begin(), i) << "] = ";
		if (i->getConsumers().size() == 0)
		{
			os << "NULL;" << endl;
		}
		else
		{
			os << "malloc(sizeof(size_t) * " << i->getConsumers().size() << ");" << endl;
			size_t counter = 0;
			for(set<const Link*>::const_iterator j = i->getConsumers().begin(); j != i->getConsumers().end(); j++)
			{
				os << "	schedule->consumers_id[" << std::distance(tg->getTasks().begin(), i) << "][" << counter << "] = " << std::distance(tg->getTasks().begin(), tg->getTasks().find(*(*j)->getConsumer())) + 1 << ";" << endl;
				counter++;
			}
		}
	}

	os << endl
		<< "	schedule->schedule = malloc(sizeof(drake_schedule_task_t*) * schedule->core_number);" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	schedule->schedule[" << i->first - 1 << "] = malloc(sizeof(drake_schedule_task_t) * " << i->second.size() << ");" << endl;

		size_t counter = 0;
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			os << "	schedule->schedule[" << i->first - 1 << "][" << counter << "].id = " << std::distance(tg->getTasks().begin(), tg->getTasks().find(*j->second.first)) + 1 << ";" << endl
				<< "	schedule->schedule[" << i->first - 1 << "][" << counter << "].start_time = " << j->second.first->getStartTime() << ";" << endl
				<< "	schedule->schedule[" << i->first - 1 << "][" << counter << "].frequency = " << j->second.first->getFrequency() << ";" << endl;
			counter++;
		}
	}

	os << "}" << endl << endl << "void drake_schedule_destroy(drake_schedule_t* schedule)" << endl << "{" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	free(schedule->schedule[" << i->first - 1 << "]);" << endl;
		/*
		for(size_t j = 0; j < i->second.size(); j++)
		{
			os << "	free(schedule->schedule[" << i->first - 1 << "][" << i->second.size() - j - 1 << "]);" << endl;
		}
		*/
	}

	os << endl << "	free(schedule->schedule);" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	free(schedule->consumers_id[" << std::distance(tg->getTasks().begin(), i) << "]);" << endl; 
		/*
		for(size_t j = 0; j < i->getConsumers().size(); j++)
		{
			os << "	free(schedule->consumers_id[" << std::distance(tg->getTasks().begin(), i) << "][" << i->getConsumers().size() - j - 1 << "]);" << endl;
		}
		*/
	}

	os << "	free(schedule->consumers_id);"
		<< endl
		<< endl;
	
	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "	free(schedule->producers_id[" << std::distance(tg->getTasks().begin(), i) << "]);" << endl; 
		/*
		for(size_t j = 0; j < i->getProducers().size(); j++)
		{
			os << "	free(schedule->producers_id[" << std::distance(tg->getTasks().begin(), i) << "][" << i->getProducers().size() - j - 1 << "]);" << endl;
		}
		*/
	}

	os << "	free(schedule->producers_id);" << endl
		<< "	free(schedule->remote_producers_in_task);" << endl
		<< "	free(schedule->remote_consumers_in_task);" << endl
		<< "	free(schedule->producers_in_task);" << endl
		<< "	free(schedule->consumers_in_task);" << endl
		<< "	free(schedule->producers_in_core);" << endl
		<< "	free(schedule->consumers_in_core);" << endl
		<< "	free(schedule->tasks_in_core);" << endl
		<< "	free(schedule->task_name);" << endl
		<< "}" << endl
		<< endl
		<< "void*" << endl
		<< "drake_function(size_t id, task_status_t status)" << endl
		<< "{" << endl
		<< "	switch(id)" << endl
		<< "	{" << endl
		<< "		default:" << endl
		<< "			// TODO: Raise an alert" << endl
		<< "		break;" << endl;

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		os << "		case " << std::distance(tg->getTasks().begin(), tg->getTasks().find(*i)) + 1<< ":" << endl
			<< "			switch(status)" << endl
			<< "			{" << endl
			<< "				case TASK_INVALID:" << endl
			<< "					return 0;" << endl
			<< "				break;" << endl
			<< "				case TASK_INIT:" << endl
			<< "					return (void*)&drake_init(" << i->getModule() << ", " << i->getName() << ");" << endl
			<< " 				break;" << endl
			<< "  				case TASK_START:" << endl
			<< "  					return (void*)&drake_start(" << i->getModule() << ", " << i->getName() << ");" << endl
			<< "  				break;" << endl
			<< "  				case TASK_RUN:" << endl
			<< "  					return (void*)&drake_run(" << i->getModule() << ", " << i->getName() << ");" << endl
			<< "  				break;" << endl
			<< "  				case TASK_KILLED:" << endl
			<< "  					return (void*)&drake_kill(" << i->getModule() << ", " << i->getName() << ");" << endl
			<< "  				break;" << endl
			<< "  				case TASK_ZOMBIE:" << endl
			<< "  					return 0;" << endl
			<< "  				break;" << endl
			<< "  				case TASK_DESTROY:" << endl
			<< "  					return (void*)&drake_destroy(" << i->getModule() << ", " << i->getName() << ");" << endl
			<< "  				break;" << endl
			<< "  				default:" << endl
			<< "  					return 0;" << endl
			<< "  				break;" << endl
			<< "  			}" << endl
			<< "  		break;" << endl;
	}

	os << endl
		<< "	}" << endl
		<< endl
		<< "	return 0;" << endl
		<< "}" << endl;
}

DrakeCSchedule*
DrakeCSchedule::clone() const
{
	return new DrakeCSchedule();
}
