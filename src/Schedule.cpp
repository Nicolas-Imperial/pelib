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


#include <set>
#include <string>

#include <pelib/Schedule.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Set.hpp>
#include <pelib/CastException.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif 

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << (#expr) << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Schedule::Schedule(const std::string &name, const std::string &appName, const table &schedule)
	{
		this->name = name;
		this->appName = appName;
	
		// Copy taskgraph	
		this->setSchedule(schedule);
	}

	Schedule::~Schedule()
	{
		// Do nothing
	}

	void
	Schedule::setSchedule(const table &schedule)
	{
		this->schedule = Schedule::table();
		this->tasks.clear();
		this->core_tasks.clear();

		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			set<const Task*> this_core_tasks;
			sequence new_sequence;
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				const Task *task = j->second.first;
				Task t = *task;
				this->tasks.insert(t);
				const Task &task_ref = *(this->getTasks().find(t));
				pair<float, work> new_pair = pair<float, work>(j->first, work(&task_ref, j->second.second));
				new_sequence.insert(new_pair);

				// Insert a reference to this task in the core's task list
				this_core_tasks.insert(&task_ref);
			}

			this->schedule.insert(pair<int, sequence>(i->first, new_sequence));

			// Add the tasks list to the list of lists of tasks
			this->core_tasks.insert(pair<int, set<const Task*> >(i->first, this_core_tasks));
		}
	}

	Schedule::Schedule(const std::string &name, const Algebra &algebra)
	{
		buildFromAlgebra(name, string("Generated from Algebra"), algebra);	
	}

	Schedule::Schedule(const std::string &name, const string &appName, const Algebra &algebra)
	{
		buildFromAlgebra(name, appName, algebra);
	}

	Schedule::Schedule(const Schedule& src)
	{
		this->name = src.getName();
		this->appName = src.getName();
		this->tasks = src.getTasks();
		this->setSchedule(src.getSchedule());
	}

	void
	Schedule::buildFromAlgebra(const string &name, const string &appName, const Algebra &algebra)
	{
		this->name = name;
		this->appName = appName;

		const Vector<int, float> *tau = algebra.find<Vector<int, float> >("Tau");
		const Vector<int, float> *start = algebra.find<Vector<int, float> >("start");
		const Vector<int, float> *wi = algebra.find<Vector<int, float> >("wi");
		const Matrix<int, int, float> *sched = algebra.find<Matrix<int, int, float> >("schedule");
		const Vector<int, float> *freq = algebra.find<Vector<int, float> >("frequency");
		const Vector<int, string> *task_name = algebra.find<Vector<int, string> >("name");

		table schedule;
		set<Task> tasks;
	
		if(start == NULL || tau == NULL || wi == NULL || sched == NULL || freq == NULL || task_name == NULL)
		{
			throw CastException("Missing some input data structure instance. Check start, Tau, wi, schedule, frequency and name.");
		}
		else
		{
			for(map<int, map<int, float> >::const_iterator i = sched->getValues().begin(); i != sched->getValues().end(); i++)
			{
				sequence core_schedule;
				
				for(map<int, float>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					if(floor(j->second) > 0)
					{
						string task_str;
						if(task_name == NULL)
						{
							stringstream estr;
							estr << "task_" << j->second;
							task_str = string(estr.str());
						}
						else
						{
							task_str = string(task_name->getValues().find((int)j->second)->second);
						}

						tasks.insert(Task(task_str));
						Task &task = (Task&)*tasks.find(task_str);
						task.setModule("dummy");

						if(task.getWorkload() > 0)
						{
							task.setWidth(wi->getValues().find((int)floor(j->second))->second);
							task.setFrequency(freq->getValues().find((int)floor(j->second))->second);
							task.setWorkload(tau->getValues().find((int)floor(j->second))->second);
							task.setStartTime(start->getValues().find((int)j->second)->second);
							core_schedule.insert(pair<float, work>(task.getStartTime(), work(&task, tau->getValues().find((int)floor(j->second))->second)));
						}
					}
				}

				schedule.insert(pair<int, sequence>(i->first, core_schedule));
			}
		}

		for(Schedule::table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			for(Schedule::sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				string taskid = j->second.first->getName();
			}
		}

		this->tasks = tasks;
		this->setSchedule(schedule);
	}
	
	Schedule*
	Schedule::clone() const
	{
		Schedule *clone = new Schedule(getName(), getName(), getSchedule());

		return clone;
	}

	Algebra
	Schedule::buildAlgebra() const
	{
		Algebra algebra;
		
		map<string, int> taskid2id; 
		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				taskid2id.insert(pair<string, int>(j->second.first->getName(), std::distance(this->getTasks().begin(), this->getTasks().find(*j->second.first)) + 1));
			}
		}

		map<int, map<int, float> > sched;
		map<int, float> frequencies, start;
		table schedule = getSchedule();

		size_t ordering, max_tasks = 0;
		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			map<int, float> schedule_row;
			int core = i->first;
	
			ordering = 0;
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, ordering++)
			{
				int id = taskid2id.find(j->second.first->getName())->second;
				schedule_row.insert(pair<int, int>(ordering, id));
				frequencies.insert(pair<int, float>(id, j->second.first->getFrequency()));

				max_tasks = ordering > max_tasks ? ordering : max_tasks;
				start.insert(pair<int, float>(std::distance(this->getTasks().begin(), this->getTasks().find(j->second.first->getName())), j->second.first->getStartTime())); 
			}

			sched.insert(pair<int, map<int, float> >(core, schedule_row));
		}

		// pad with 0
		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			size_t base = i->second.size();
			for(ordering = base; ordering < max_tasks; ordering++)
			{
				sched.find(i->first)->second.insert(pair<int, int>(ordering, 0));
			}
		}

		Matrix<int, int, float> ampl_schedule("schedule", sched);
		Vector<int, float> ampl_frequencies("frequency", frequencies);
		Vector<int, float> ampl_start("start", start);

		algebra.insert(&ampl_schedule);
		algebra.insert(&ampl_frequencies);
		algebra.insert(&ampl_start);

		return algebra;
	}

	std::string
	Schedule::getName() const
	{
		return this->name;
	}

	std::string
	Schedule::getAppName() const
	{
		return this->appName;
	}

	const Schedule::table&
	Schedule::getSchedule() const
	{
		return schedule;
	}
	
	const set<Task>&
	Schedule::getTasks() const
	{
		return this->tasks;
	}

	const Task&
	Schedule::getTask(int id) const
	{
		set<Task>::const_iterator it = this->getTasks().begin();
		std::advance(it, id - 1);
		return *it;
	}

	Schedule&
	Schedule::operator=(const Schedule &copy)
	{
		this->name = name;
		this->appName = appName;
		this->tasks = copy.getTasks();
	
		// Copy taskgraph	
		this->setSchedule(schedule);

		return *this;
	}

	const set<const Task*>&
	Schedule::getTasks(int core) const
	{
		return this->core_tasks.find(core)->second;
	}

	set<const Task*>
	Schedule::getTasksSharedMemoryIsland(const set<int>& islands, const Platform &pt) const
	{
		set<const Task*> tasks;

		for(set<int>::const_iterator i = islands.begin(); i != islands.end(); i++)
		{
			set<Platform::island>::const_iterator ii = pt.getSharedMemoryIslands().begin();
			std::advance(ii, *i - 1);

			for(Platform::island::const_iterator j = ii->begin(); j != ii->end(); j++)
			{
				size_t core_id = std::distance(pt.getCores().begin(), pt.getCores().find(*j)) + 1;
				for(set<const Task*>::const_iterator k = this->getTasks((int)core_id).begin(); k !=  this->getTasks((int)core_id).end(); k++)
				{
					tasks.insert(*k);
				}
			}
		}

		return tasks;
	}

	multiset<const Task*>
	Schedule::getRemoteSharedMemoryIslandProducers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const
	{
		multiset<const Task*> producers;
		for(set<int>::const_iterator i = islands.begin(); i != islands.end(); i++)
		{
			if(*i > (int)pt.getSharedMemoryIslands().size())
			{
				throw CastException("Requested shared memory islands not existing in platform.");
			}
		}

		set<const Task*> tasks_in_islands = this->getTasksSharedMemoryIsland(islands, pt);
		for(set<const Task*>::const_iterator i = tasks_in_islands.begin(); i != tasks_in_islands.end(); i++)
		{
			const Task *task_p = *i;
			if(tg.getTasks().find(*task_p) == tg.getTasks().end())
			{
				stringstream ss;
				ss << "Cannot find task \"" << task_p->getName() << "\" in taskgraph.";
				throw CastException(ss.str());
			}
			const Task &task_tg = *tg.getTasks().find(*task_p);
			set<int> consumer_cores = this->getCores(task_tg);
			set<int>::const_iterator j = consumer_cores.begin();
			set<Platform::island> consumer_core_islands = pt.getSharedMemoryIslands(*j);
			for(; j != consumer_cores.end(); j++)
			{
				if(pt.getSharedMemoryIslands(*j) != consumer_core_islands)
				{
					throw CastException("Task mapped to cores that belong to different shared memory islands.");
				}	
			}
			
			for(set<const Link*>::const_iterator j = task_tg.getProducers().begin(); j != task_tg.getProducers().end(); j++)
			{
				const Link *l = *j;
				const Task *producer = l->getProducer();
				set<int> producer_cores = this->getCores(*producer);
				set<int>::const_iterator k = producer_cores.begin();
				set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*k);
				for(; k != producer_cores.end(); k++)
				{
					if(pt.getSharedMemoryIslands(*k) != producer_core_islands)
					{
						throw CastException("Task mapped to cores that belong to different shared memory islands.");
					}	
				}

				if(consumer_core_islands != producer_core_islands)
				{
					producers.insert(producer);
				}
			}
		}

		return producers;
	}

	multiset<const Task*>
	Schedule::getRemoteSharedMemoryIslandTaskProducers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		multiset<const Task*> producers;
		if(this->getTasks().find(t) == this->getTasks().end())
		{
			stringstream ss;
			ss << "Task \"" << t.getName() << "\" does not figure in schedule.";
			throw CastException(ss.str());
		}

		if(tg.getTasks().find(t) == tg.getTasks().end())
		{
			stringstream ss;
			ss << "Task \"" << t.getName() << "\" does not figure in taskgraph.";
			throw CastException(ss.str());
		}

		const Task &task_tg = *tg.getTasks().find(t);
		set<int> consumer_cores = this->getCores(task_tg);
		set<int>::const_iterator j = consumer_cores.begin();
		set<Platform::island> consumer_core_islands = pt.getSharedMemoryIslands(*j);
		for(; j != consumer_cores.end(); j++)
		{
			if(pt.getSharedMemoryIslands(*j) != consumer_core_islands)
			{
				throw CastException("Task mapped to cores that belong to different shared memory islands.");
			}	
		}
	
		for(set<const Link*>::const_iterator j = task_tg.getProducers().begin(); j != task_tg.getProducers().end(); j++)
		{
			const Link *l = *j;
			const Task *producer = l->getProducer();
			set<int> producer_cores = this->getCores(*producer);
			set<int>::const_iterator k = producer_cores.begin();

			set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*k);
			for(; k != producer_cores.end(); k++)
			{
				if(pt.getSharedMemoryIslands(*k) != producer_core_islands)
				{
					throw CastException("Task mapped to cores that belong to different shared memory islands.");
				}	
			}

			if(consumer_core_islands != producer_core_islands)
			{
				producers.insert(producer);
			}
		}

		return producers;
	}

	multiset<const Task*>
	Schedule::getRemoteTaskConsumers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		set<Task>::const_iterator iter = tg.getTasks().find(t);
		if(iter == tg.getTasks().end())
		{
			throw PelibException("Task in schedule does not exist in taskgraph.");
		}
		const Task &task = *iter;	

		multiset<const Task*> consumers;
		for(set<const Link*>::const_iterator j = task.getConsumers().begin(); j != task.getConsumers().end(); j++)
		{
			const Task *consumer = (*j)->getConsumer();
			if(this->getCores(t) != this->getCores(*consumer))
			{
				consumers.insert(consumer);
			}
		}
	
		return consumers;
	}

	multiset<const Task*>
	Schedule::getRemoteTaskProducers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		set<Task>::const_iterator iter = tg.getTasks().find(t);
		if(iter == tg.getTasks().end())
		{
			throw PelibException("Task in schedule does not exist in taskgraph.");
		}
		const Task &task = *iter;	

		multiset<const Task*> producers;
		for(set<const Link*>::const_iterator j = task.getProducers().begin(); j != task.getProducers().end(); j++)
		{
			const Task *producer = (*j)->getProducer();
			if(this->getCores(t) != this->getCores(*producer))
			{
				producers.insert(producer);
			}
		}
	
		return producers;
	}

	multiset<const Task*>
	Schedule::getRemoteConsumers(int core, const Taskgraph &tg, const Platform &pt) const
	{
		table::const_iterator iter = this->getSchedule().find(core);
		if(iter == this->getSchedule().end())
		{
			throw PelibException("Cannot find requested core in schedule");
		}

		const sequence &seq = iter->second;

		multiset<const Task*> consumers;
		for(sequence::const_iterator i = seq.begin(); i != seq.end(); i++)
		{
			set<Task>::const_iterator iter = tg.getTasks().find(*i->second.first);
			if(iter == tg.getTasks().end())
			{
				throw PelibException("Task in schedule does not exist in taskgraph.");
			}
			const Task &task = *iter;	

			for(set<const Link*>::const_iterator j = task.getConsumers().begin(); j != task.getConsumers().end(); j++)
			{
				const Task *consumer = (*j)->getConsumer();
				if(this->getCores(*i->second.first) != this->getCores(*consumer))
				{
					consumers.insert(consumer);
				}
			}
		}
	
		return consumers;
	}

	multiset<const Task*>
	Schedule::getRemoteProducers(int core, const Taskgraph &tg, const Platform &pt) const
	{
		table::const_iterator iter = this->getSchedule().find(core);
		if(iter == this->getSchedule().end())
		{
			throw PelibException("Cannot find requested core in schedule");
		}

		const sequence &seq = iter->second;

		multiset<const Task*> producers;
		for(sequence::const_iterator i = seq.begin(); i != seq.end(); i++)
		{
			set<Task>::const_iterator iter = tg.getTasks().find(*i->second.first);
			if(iter == tg.getTasks().end())
			{
				throw PelibException("Task in schedule does not exist in taskgraph.");
			}
			const Task &task = *iter;	

			for(set<const Link*>::const_iterator j = task.getProducers().begin(); j != task.getProducers().end(); j++)
			{
				const Task *producer = (*j)->getProducer();
				if(this->getCores(*i->second.first) != this->getCores(*producer))
				{
					producers.insert(producer);
				}
			}
		}
	
		return producers;
	}

	multiset<const Task*>
	Schedule::getRemoteSharedMemoryIslandConsumers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const
	{
		multiset<const Task*> consumers;
		for(set<int>::const_iterator i = islands.begin(); i != islands.end(); i++)
		{
			if(*i > (int)pt.getSharedMemoryIslands().size())
			{
				throw CastException("Requested shared memory islands not existing in platform.");
			}
		}

		set<const Task*> tasks_in_islands = this->getTasksSharedMemoryIsland(islands, pt);
		for(set<const Task*>::const_iterator i = tasks_in_islands.begin(); i != tasks_in_islands.end(); i++)
		{
			const Task *task_p = *i;
			if(tg.getTasks().find(*task_p) == tg.getTasks().end())
			{
				stringstream ss;
				ss << "Trying to schedule task \"" << task_p->getName() << "\" that does not exist in taskgraph.";
				throw CastException(ss.str());
			}
			const Task &task_tg = *tg.getTasks().find(*task_p);
			set<int> producer_cores = this->getCores(task_tg);
			set<int>::const_iterator j = producer_cores.begin();
			set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*j);
			for(; j != producer_cores.end(); j++)
			{
				if(pt.getSharedMemoryIslands(*j) != producer_core_islands)
				{
					throw CastException("Task mapped to cores that belong to different shared memory islands.");
				}	
			}
			
			for(set<const Link*>::const_iterator j = task_tg.getConsumers().begin(); j != task_tg.getConsumers().end(); j++)
			{
				const Link *l = *j;
				const Task *consumer = l->getConsumer();
				set<int> consumer_cores = this->getCores(*consumer);
				set<int>::const_iterator k = consumer_cores.begin();

				set<Platform::island> consumer_core_islands = pt.getSharedMemoryIslands(*k);
				for(; k != consumer_cores.end(); k++)
				{
					if(pt.getSharedMemoryIslands(*k) != consumer_core_islands)
					{
						throw CastException("Task mapped to cores that belong to different shared memory islands.");
					}	
				}

				if(producer_core_islands != consumer_core_islands)
				{
					consumers.insert(consumer);
				}
			}
		}

		return consumers;
	}

	multiset<const Task*>
	Schedule::getRemoteSharedMemoryIslandTaskConsumers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		multiset<const Task*> consumers;
		if(this->getTasks().find(t) == this->getTasks().end())
		{
			stringstream ss;
			ss << "Task \"" << t.getName() << "\" does not figure in schedule.";
			throw CastException(ss.str());
		}

		if(tg.getTasks().find(t) == tg.getTasks().end())
		{
			stringstream ss;
			ss << "Task \"" << t.getName() << "\" does not figure in taskgraph.";
			throw CastException(ss.str());
		}

		const Task &task_tg = *tg.getTasks().find(t);
		set<int> producer_cores = this->getCores(task_tg);
		set<int>::const_iterator j = producer_cores.begin();
		set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*j);
		for(; j != producer_cores.end(); j++)
		{
			if(pt.getSharedMemoryIslands(*j) != producer_core_islands)
			{
				throw CastException("Task mapped to cores that belong to different shared memory islands.");
			}	
		}
		
		for(set<const Link*>::const_iterator j = task_tg.getConsumers().begin(); j != task_tg.getConsumers().end(); j++)
		{
			const Link *l = *j;
			const Task *consumer = l->getConsumer();
			set<int> consumer_cores = this->getCores(*this->getTasks().find(*consumer));
			set<int>::const_iterator k = consumer_cores.begin();

			set<Platform::island> consumer_core_islands = pt.getSharedMemoryIslands(*k);
			for(; k != consumer_cores.end(); k++)
			{
				if(pt.getSharedMemoryIslands(*k) != consumer_core_islands)
				{
					throw CastException("Task mapped to cores that belong to different shared memory islands.");
				}	
			}

			if(producer_core_islands != consumer_core_islands)
			{
				consumers.insert(consumer);
			}
		}

		return consumers;
	}

	const set<int>
	Schedule::getCores(const Task &t) const
	{
		if(this->getTasks().find(t) == this->getTasks().end())
		{
			stringstream ss;
			ss << "Task \"" << t.getName() << "\" does not figure in schedule.";
			throw PelibException(ss.str());
		}
		const Task &tt = *this->getTasks().find(t);

		set<int> cores;
		for(table::const_iterator i = this->getSchedule().begin(); i != this->getSchedule().end(); i++)
		{
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				if(*(j->second.first) == tt)
				{
					cores.insert(i->first);
				}
			}
		}

		return cores;
	}

	Algebra
	Schedule::addStartTime(const Algebra &data, const Taskgraph &tg, const Platform &platform)
	{
		// Now browse all tasks in all cores and compute a starting time matrix
		Algebra sched = data;
		Algebra input = tg.buildAlgebra(platform);
		input = input.merge(platform.buildAlgebra());
		input = input.merge(sched);

		map<int, float> start_time = input.find<Vector<int, float> >("Tau")->getValues();
		map<int, float> frequency = input.find<Vector<int, float> >("frequency")->getValues();
		map<int, float> workload = input.find<Vector<int, float> >("Tau")->getValues();
		map<int, float> width = input.find<Vector<int, float> >("wi")->getValues();
		map<int, float> max_width = input.find<Vector<int, float> >("Wi")->getValues();
		map<int, map<int, float> > efficiency = input.find<Matrix<int, int, float> >("e")->getValues();
		map<int, map<int, float> > map_sched = input.find<Matrix<int, int, float> >("schedule")->getValues();

		for(map<int, float>::iterator i = start_time.begin(); i != start_time.end(); i++)
		{
			i->second = 0;
		}

		for(map<int, map<int, float> >::const_iterator i = map_sched.begin(); i != map_sched.end(); i++)
		{
			double mystart = 0;
			for(map<int, float>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				int n = (int)j->second;
				if(n > 0)
				{
					if(start_time.find((int)n)->second < mystart)
					{
						start_time.find((int)n)->second = mystart;
					}

					double tau = workload.find((int)n)->second;
					size_t wi = (size_t)width.find((int)n)->second;
					size_t Wi = (size_t)max_width.find((int)n)->second;
					double e = wi <= Wi ? efficiency.find((int)n)->second.find((int)wi)->second : 1e-06;
					double f = frequency.find((int)n)->second;
					double delta = (tau / (wi * e) / f);
					mystart = mystart + delta;
				}
			}
		}

		const Vector<int, string> *name = input.find<Vector<int, string> >("name");
		if(name == NULL)
		{
			map<int, string> names;
			for(set<Task>::const_iterator iter = tg.getTasks().begin(); iter != tg.getTasks().end(); iter++)
			{
				names.insert(pair<int, string>(std::distance(tg.getTasks().begin(), iter) + 1, iter->getName()));
			}

			name = new Vector<int, string>("name", names);
			sched.insert(name);
		}

		Vector<int, float> *start = new Vector<int, float>("start", start_time);
		sched.insert(start);

		return sched;
	}
}
