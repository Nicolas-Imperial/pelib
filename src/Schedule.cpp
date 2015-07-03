#include <set>
#include <string>

#include <AmplOutput.hpp>

#include <Schedule.hpp>
#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <CastException.hpp>

using namespace std;

/*
#if DEBUG
#define trace(var) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"." << endl
#else
#define trace(var)
#endif
*/
#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;

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
				Task &task_ref = (Task&)*this->getTasks().find(t);
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
		this->name = name;
		this->appName = string("Generated from Algebra");

		//const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Vector<int, float> *tau = algebra.find<Vector<int, float> >("Tau");
		const Vector<int, float> *start = algebra.find<Vector<int, float> >("start");
		const Vector<int, float> *wi = algebra.find<Vector<int, float> >("wi");
		//const Vector<int, float> *Wi = algebra.find<Vector<int, float> >("Wi");
		const Matrix<int, int, float> *sched = algebra.find<Matrix<int, int, float> >("schedule");
		//const Matrix<int, int, float> *e = algebra.find<Matrix<int, int, float> >("e");
		const Vector<int, float> *freq = algebra.find<Vector<int, float> >("frequency");

		table schedule;
		set<Task> tasks;
	
		if(start == NULL || tau == NULL || wi == NULL || sched == NULL || freq == NULL)
		{
			throw CastException("Missing parameter");
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
						stringstream estr;
						estr << "task_" << j->second;
						tasks.insert(Task(estr.str()));
						Task &task = (Task&)*tasks.find(estr.str());
						task.setModule("dummy");
						//Task task((int)j->second, tg.findTask((int)floor(j->second)).getTaskId());

						//task.setWorkload(tau->getValues().find((int)floor(j->second))->second);
						if(task.getWorkload() > 0)
						{
							//Task& task_tg = (Task&)*tg.getTasks().find(task);
							task.setWidth(wi->getValues().find((int)floor(j->second))->second);
							task.setFrequency(freq->getValues().find((int)floor(j->second))->second);
							task.setWorkload(tau->getValues().find((int)floor(j->second))->second);
							//task_tg.setMaxWidth(tg.getTasks().find(task)->getMaxWidth());
							//task_tg.setMaxWidth(Wi->getValues().find((int)floor(j->second))->second);

							/*
							stringstream estr;
							for(Matrix<int, int, float>::RowType::const_iterator k = e->getValues().find((int)j->second)->second.begin(); k != e->getValues().find((int)j->second)->second.end(); k++)
							{
								estr << k->second << " ";
							}
							task_tg.setEfficiencyString(estr.str());
							*/
							//task_tg.setEfficiencyString(tg.getTasks().find(task)->getEfficiencyString());

							task.setStartTime(start->getValues().find((int)j->second)->second);

							//cerr << "Core: " << i->first << "; Task: " << j->second << "; taskid: " << tg.findTask((int)floor(j->second)).getTaskId() << "; Start time: " << task.getStartTime() << endl;
							//const Task &task_ref = *this->getTaskgraph().getTasks().find(task);
							//core_schedule.insert(pair<float, Task*>(task.getStartTime(), &(this->getTasks().find(task))));
							core_schedule.insert(pair<float, work>(task.getStartTime(), work(&task, tau->getValues().find((int)floor(j->second))->second)));
						}
					}
				}

				schedule.insert(pair<int, sequence>(i->first, core_schedule));
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

			sched.insert(pair<int, map<int, float> >(core + 1, schedule_row));
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
			std::advance(ii, *i);

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

	set<const Task*>
	Schedule::getRemoteSharedMemoryIslandProducers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const
	{
		set<const Task*> producers;
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
			const Task &task_tg = *tg.getTasks().find(*task_p);
			set<int> consumer_cores = this->getCores(task_tg);
			set<int>::const_iterator j = consumer_cores.begin();
			// TODO: investigate why this happens
			//if(*j == 0) debug(*j);
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
				set<int> producer_cores = this->getCores(*this->getTasks().find(*producer));
				set<int>::const_iterator k = producer_cores.begin();
				// TODO: investigate why this happens
				/*
				if(*k == 0)
				{
					debug(*k);
					debug(producer->getName());
				}
				*/
				set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*k);
				for(; k != producer_cores.end(); k++)
				{
					// TODO: investigate why this happens
					//if(*k == 0) debug(*k);
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

	set<const Task*>
	Schedule::getRemoteSharedMemoryIslandTaskProducers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		set<const Task*> producers;
		const Task &task_tg = *tg.getTasks().find(t);
		set<int> consumer_cores = this->getCores(task_tg);
		set<int>::const_iterator j = consumer_cores.begin();
		// TODO: investigate why this happens
		//if(*j == 0) debug(*j);
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
			set<int> producer_cores = this->getCores(*this->getTasks().find(*producer));
			set<int>::const_iterator k = producer_cores.begin();
			// TODO: investigate why this happens
			/*
			if (*k == 0)
			{
				debug(*k);
				debug(producer->getName());
			}
			*/

			set<Platform::island> producer_core_islands = pt.getSharedMemoryIslands(*k);
			for(; k != producer_cores.end(); k++)
			{
				// TODO: investigate why this happens
				//if(*k == 0) debug(*k);
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

	set<const Task*>
	Schedule::getRemoteSharedMemoryIslandConsumers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const
	{
		set<const Task*> consumers;
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
			const Task &task_tg = *tg.getTasks().find(*task_p);
			set<int> producer_cores = this->getCores(task_tg);
			set<int>::const_iterator j = producer_cores.begin();
			// TODO: investigate why this happens
			//if(*j == 0) debug(*j);
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
					// TODO: investigate why this happens
					//if(*k == 0) debug(*k);
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

	set<const Task*>
	Schedule::getRemoteSharedMemoryIslandTaskConsumers(const Task &t, const Taskgraph &tg, const Platform &pt) const
	{
		set<const Task*> consumers;
		const Task &task_tg = *tg.getTasks().find(t);
		set<int> producer_cores = this->getCores(task_tg);
		set<int>::const_iterator j = producer_cores.begin();
		// TODO: investigate why this happens
		//if(*j == 0) debug(*j);
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
				// TODO: investigate why this happens
				//if(*k == 0) debug(*k);
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
	Schedule::getCores(const Task t) const
	{
		set<int> cores;
		for(table::const_iterator i = this->getSchedule().begin(); i != this->getSchedule().end(); i++)
		{
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				if(*j->second.first == t)
				{
					// TODO: investigate why this happens
					//if(i->first == 0) debug(i->first);
					cores.insert(i->first);
				}
			}
		}

		return cores;
	}
}
