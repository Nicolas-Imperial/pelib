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
#include "SnekkjaCSchedule.hpp"

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <Task.hpp>

#include <CastException.hpp>
#include <ParseException.hpp>

using namespace pelib;
using namespace std;

void
SnekkjaCSchedule::dump(ostream& os, const StreamingAppData &data) const
{
	dump(os, &data);
}

void
SnekkjaCSchedule::dump(ostream& os, const StreamingAppData *data) const
{
	const Schedule *sched = dynamic_cast<const Schedule* >(data);

	// Gather all tasks in a single collection
	//const set<Task> &tasks = sched->getTaskgraph().getTasks();

	size_t p = sched->getSchedule().size();
	size_t n = sched->getTaskgraph().getTasks().size();
	map<int, set<const Task*> > tasks, producers, consumers;
	//trace(sched->getTaskgraph().getTasks().begin()->getProducers().size());
	//trace(sched->getTaskgraph().getTasks().rbegin()->getConsumers().size());
	
	os << "#include <snekkja/schedule.h> " << endl << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		const Task &t = *i;
		os <<
			"#define TASK_NAME " << t.getTaskId() << endl <<
			"#define TASK_ID " << t.getId() << endl <<
			"#include <snekkja/node.h>" << endl <<
			"#define DONE_" << t.getId() << " 1" << endl << endl;
	}

	os << 
		"size_t _snekkja_p;" << endl << 
		"size_t _snekkja_n;" << endl << 
		"size_t *_snekkja_tasks_in_core;" << endl << 
		"size_t *_snekkja_consumers_in_core;" << endl << 
		"size_t *_snekkja_producers_in_core;" << endl <<
		"snekkja_schedule_task_t **_snekkja_schedule;" << endl <<
		"size_t *_snekkja_tasks_in_core;" << endl <<
		"size_t *_snekkja_consumers_in_core;" << endl <<
		"size_t *_snekkja_producers_in_core;" << endl <<
		"size_t *_snekkja_consumers_in_task;" << endl <<
		"size_t *_snekkja_producers_in_task;" << endl <<
		"size_t *_snekkja_remote_consumers_in_task;" << endl <<
		"size_t *_snekkja_remote_producers_in_task;" << endl <<
		"size_t **_snekkja_consumers_id;" << endl <<
		"size_t **_snekkja_producers_id;" << endl <<
		"int *_snekkja_task_frequency;" << endl <<
		"double _snekkja_stage_time;" << endl << endl <<
		"void snekkja_schedule_init()" << endl <<
		"{" << endl <<
		"	_snekkja_p = " << p << ";" << endl << 
		"	_snekkja_n = " << n << ";" << endl <<
		"	_snekkja_stage_time = " << sched->getRoundTime() << ";" << endl << endl <<
		"	_snekkja_tasks_in_core = malloc(sizeof(size_t) * _snekkja_p);" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	_snekkja_tasks_in_core[" << i->first - 1 << "] = " << sched->getTasks(i->first, tasks).size() << ";\n";
	}

	os << endl << "	_snekkja_consumers_in_core = malloc(sizeof(size_t) * _snekkja_p);" << endl; 

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	_snekkja_consumers_in_core[" << i->first - 1 << "] = " << sched->getConsumers(i->first, consumers).size() << ";\n";
	}

	os << endl << "	_snekkja_producers_in_core = malloc(sizeof(size_t) * _snekkja_p);" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	_snekkja_producers_in_core[" << i->first - 1 << "] = " << sched->getProducers(i->first, producers).size() << ";\n";
	}

	os << endl << "	_snekkja_task_frequency = malloc(sizeof(size_t) * _snekkja_n);" << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "	_snekkja_task_frequency[" << i->getId() - 1 << "] = " << i->getFrequency() << ";" << endl;
	}

	os << endl << "	_snekkja_consumers_in_task = malloc(sizeof(size_t) * _snekkja_n);" << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "	_snekkja_consumers_in_task[" << i->getId() - 1 << "] = " << i->getConsumers().size() << ";" << endl;
	}

	os << endl << "	_snekkja_producers_in_task = malloc(sizeof(size_t) * _snekkja_n);" << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "	_snekkja_producers_in_task[" << i->getId() - 1 << "] = " << i->getProducers().size() << ";" << endl;
	}

	os << endl << "	_snekkja_remote_consumers_in_task = malloc(sizeof(size_t) * _snekkja_n);" << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		const Task &task = *i;
		set<int> cores = sched->getCores(&task);
		int remote_consumers = 0;

		for(set<const Link*>::const_iterator j = i->getConsumers().begin(); j != i->getConsumers().end(); j++)
		{
			set<int> consumerCores = sched->getCores((*j)->getConsumer());

			set<int> intersect;
			set_intersection(cores.begin(), cores.end(), consumerCores.begin(), consumerCores.end(), std::inserter(intersect, intersect.begin()));

			if(intersect.size() == 0)
			{
				remote_consumers++;
			}
		}

		os << "	_snekkja_remote_consumers_in_task[" << i->getId() - 1 << "] = " << remote_consumers << ";" << endl;
	}

	os << endl << "	_snekkja_remote_producers_in_task = malloc(sizeof(size_t) * _snekkja_n);" << endl;

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		const Task &task = *i;
		set<int> cores = sched->getCores(&task);
		int remote_producers = 0;

		for(set<const Link*>::const_iterator j = i->getProducers().begin(); j != i->getProducers().end(); j++)
		{
			set<int> producerCores = sched->getCores((*j)->getProducer());

			set<int> intersect;
			set_intersection(cores.begin(), cores.end(), producerCores.begin(), producerCores.end(), std::inserter(intersect, intersect.begin()));

			if(intersect.size() == 0)
			{
				remote_producers++;
			}
		}

		os << "	_snekkja_remote_producers_in_task[" << i->getId() - 1 << "] = " << remote_producers << ";\n";
	}

	os << "\
\n\
	_snekkja_producers_id = malloc(sizeof(size_t*) * _snekkja_n);\n";

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "	_snekkja_producers_id[" << i->getId() - 1 << "] = ";
		if (i->getProducers().size() == 0)
		{
			os << "NULL;" << endl;
		}
		else
		{
			os << "malloc(sizeof(size_t) * " << i->getProducers().size() << ");\n";
			size_t counter = 0;
			for(set<const Link*>::const_iterator j = i->getProducers().begin(); j != i->getProducers().end(); j++)
			{
				os << "	_snekkja_producers_id[" << i->getId() - 1 << "][" << counter << "] = " << (*j)->getProducer()->getId() << ";" << endl;
				counter++;
			}
		}
	}

	os << "\
\n\
	_snekkja_consumers_id = malloc(sizeof(size_t*) * _snekkja_n);\n";

	// TODO: iterate through all tasks
	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "	_snekkja_consumers_id[" << i->getId() - 1 << "] = ";
		if (i->getConsumers().size() == 0)
		{
			os << "NULL;" << endl;
		}
		else
		{
			os << "malloc(sizeof(size_t) * " << i->getConsumers().size() << ");\n";
			size_t counter = 0;
			for(set<const Link*>::const_iterator j = i->getConsumers().begin(); j != i->getConsumers().end(); j++)
			{
				os << "	_snekkja_producers_id[" << i->getId() - 1 << "][" << counter << "] = " << (*j)->getConsumer()->getId() << ";" << endl;
				counter++;
			}
		}
	}
	//_snekkja_consumers_id[4][0] = 2;\n

	os << "\
\n\
	_snekkja_schedule = malloc(sizeof(snekkja_schedule_task_t*) * _snekkja_p);\n";

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		os << "	_snekkja_schedule[" << "0" << "] = malloc(sizeof(snekkja_schedule_task_t) * " << "1" << ");\n";

		size_t counter = 0;
		for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			os << "	_snekkja_schedule[" << "0" << "][" << counter << "].id = " << j->second.first->getId() << ";" << endl
				<< "	_snekkja_schedule[" << "0" << "][" << counter << "].start_time = " << j->second.first->getStartTime() << ";" << endl
				<< "	_snekkja_schedule[" << "0" << "][" << counter << "].frequency = " << j->second.first->getFrequency() << ";" << endl;
			counter++;
		}
	}

	os << "}" << endl << endl << "void snekkja_schedule_destroy()" << endl << "{" << endl;

	for(Schedule::table::const_iterator i = sched->getSchedule().begin(); i != sched->getSchedule().end(); i++)
	{
		for(size_t j = 0; j < i->second.size(); j++)
		{
			os << "	free(_snekkja_schedule[" << i->second.size() - j - 1 << "]);\n";
		}
	}

	os << "\
	free(_snekkja_schedule);\n\
	free(_snekkja_task_frequency);\n";

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		for(size_t j = 0; j < i->getConsumers().size(); j++)
		{
			os << "	free(_snekkja_consumers_id[" << i->getConsumers().size() - j - 1 << "]);" << endl;
		}
	}

	os << "	free(_snekkja_consumers_id);" << endl;
	
	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		for(size_t j = 0; j < i->getProducers().size(); j++)
		{
			os << "	free(_snekkja_producers_id[" << i->getProducers().size() - j - 1 << "]);" << endl;
		}
	}

	os << "\
	free(_snekkja_producers_id);\n\
	free(_snekkja_remote_producers_in_task);\n\
	free(_snekkja_remote_consumers_in_task);\n\
	free(_snekkja_producers_in_task);\n\
	free(_snekkja_consumers_in_task);\n\
	free(_snekkja_producers_in_core);\n\
	free(_snekkja_consumers_in_core);\n\
	free(_snekkja_tasks_in_core);\n\
}\n\
\n\
void*\n\
snekkja_function(size_t id, task_status_t status)\n\
{\n\
	switch(id)\n\
	{\n\
		default:\n";

	for(set<Task>::const_iterator i = sched->getTaskgraph().getTasks().begin(); i != sched->getTaskgraph().getTasks().end(); i++)
	{
		os << "\
		case " << i->getId() << ":\n\
			switch(status)\n\
			{\n\
				case TASK_INVALID:\n\
					return 0;\n\
				break;\n\
				case TASK_INIT:\n\
					return (void*)&snekkja_init(" << i->getName() << ", " << i->getName() << ");\n\
				break;\n\
				case TASK_START:\n\
					return (void*)&snekkja_start(" << i->getName() << ", " << i->getName() << ");\n\
				break;\n\
				case TASK_RUN:\n\
					return (void*)&snekkja_run(" << i->getName() << ", " << i->getName() << ");\n\
				break;\n\
				case TASK_KILLED:\n\
					return (void*)&snekkja_destroy(" << i->getName() << ", " << i->getName() << ");\n\
				break;\n\
				case TASK_ZOMBIE:\n\
					return 0;\n\
				break;\n\
				default:\n\
					return 0;\n\
				break;\n\
			}\n\
		break;\n";
	}

	os << "\
	}\n\
\n\
	return 0;\n\
}\n\
";
	/* TODO: implement
	if(sched == NULL) throw CastException("parameter \"data\" was not of type \"Schedule*\".");

	map<int, std::map<float, Task> > schedule = sched->getSchedule();

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

	os << "</schedule>\n"i;
	*/
}

SnekkjaCSchedule*
SnekkjaCSchedule::clone() const
{
	return new SnekkjaCSchedule();
}
