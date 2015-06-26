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

namespace pelib
{
	Schedule::Schedule(const std::string &name, const std::string &appName)
	{
		roundTime = 0;
		this->name = name;
		this->appName = appName;
	}

	Schedule::Schedule(const std::string &name, const std::string &appName, const Taskgraph &taskgraph, const table &schedule, double roundTime)
	{
		this->name = name;
		this->appName = appName;
		roundTime = roundTime;
	
		// Copy taskgraph	
		this->taskgraph = taskgraph;
		this->setSchedule(schedule);
	}

	Schedule::~Schedule()
	{
		// Do nothing
	}

	void
	Schedule::setSchedule(const table &schedule)
	{
		this->schedule = schedule;

		for(table::iterator i = this->schedule.begin(); i != this->schedule.end(); i++)
		{
			for(sequence::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				const Task *task = j->second.first;
				Task t = *task;
				Task &task_ref = (Task&)*this->getTaskgraph().getTasks().find(t);
				pair<float, work> new_pair = pair<float, work>(j->first, work(&task_ref, j->second.second));
				i->second.erase(j);
				i->second.insert(new_pair);
			}
		}
	}

	Schedule::Schedule(const std::string &name, const Taskgraph &taskgraph, const Algebra &data)
	{
		Taskgraph tg = taskgraph;
		roundTime = 0;
		this->name = name;
		this->appName = tg.getName();

		Algebra algebra = data.merge(tg.buildAlgebra());
		
		const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Vector<int, float> *tau = algebra.find<Vector<int, float> >("Tau");
		const Vector<int, float> *starting_time = algebra.find<Vector<int, float> >("starting_time");
		const Vector<int, float> *wi = algebra.find<Vector<int, float> >("wi");
		const Matrix<int, int, float> *sched = algebra.find<Matrix<int, int, float> >("schedule");
		const Vector<int, float> *freq = algebra.find<Vector<int, float> >("frequency");

		table schedule;
		
		if(M == NULL || tau == NULL || wi == NULL || sched == NULL || freq == NULL)
		{
			throw CastException("Missing parameter");
		}
		else
		{
			this->roundTime = M->getValue();

			for(map<int, map<int, float> >::const_iterator i = sched->getValues().begin(); i != sched->getValues().end(); i++)
			{
				sequence core_schedule;
				float start = 0;
				
				for(map<int, float>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					if(floor(j->second) > 0)
					{
						Task task((int)j->second, tg.findTask((int)floor(j->second)).getTaskId());
						//task.setWorkload(tau->getValues().find((int)floor(j->second))->second);
						if(task.getWorkload() > 0)
						{
							Task& task_tg = (Task&)*tg.getTasks().find(task);

							task_tg.setWidth(wi->getValues().find((int)floor(j->second))->second);
							task_tg.setFrequency(freq->getValues().find((int)floor(j->second))->second);
							task_tg.setMaxWidth(tg.getTasks().find(task)->getMaxWidth());
							task_tg.setEfficiencyString(tg.getTasks().find(task)->getEfficiencyString());

							if(starting_time == NULL)
							{
								task_tg.setStartTime(start);
							}
							else
							{
								task_tg.setStartTime(starting_time->getValues().find((int)j->second)->second);
							}

							//cerr << "Core: " << i->first << "; Task: " << j->second << "; taskid: " << tg.findTask((int)floor(j->second)).getTaskId() << "; Start time: " << task.getStartTime() << endl;
							//const Task &task_ref = *this->getTaskgraph().getTasks().find(task);
							//core_schedule.insert(pair<float, Task*>(task.getStartTime(), &(this->getTasks().find(task))));
							core_schedule.insert(pair<float, work>(task_tg.getStartTime(), work(&task_tg, tau->getValues().find((int)floor(j->second))->second)));
							start += task_tg.runtime(task_tg.getWidth(), task_tg.getFrequency());
						}
					}
				}

				schedule.insert(pair<int, sequence>(i->first, core_schedule));
			}
		}

		this->taskgraph = taskgraph;
		this->taskgraph = tg;
		this->setSchedule(schedule);
	}
	
	Schedule*
	Schedule::clone() const
	{
		Schedule *clone = new Schedule(getName(), getName(), getTaskgraph(), getSchedule(), getRoundTime());

		return clone;
	}

	Algebra
	Schedule::buildAlgebra() const
	{
		Algebra algebra;
		
		map<string, int>taskid2id; 
		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				taskid2id.insert(pair<string, int>(j->second.first->getTaskId(), j->second.first->getId()));
			}
		}

		map<int, map<int, float> > sched;
		map<int, float> frequencies;
		table schedule = getSchedule();

		size_t ordering, max_tasks = 0;
		for(table::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			map<int, float> schedule_row;
			int core = i->first;
	
			ordering = 0;
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++, ordering++)
			{
				int id = taskid2id.find(j->second.first->getTaskId())->second;
				schedule_row.insert(pair<int, int>(ordering, id));
				frequencies.insert(pair<int, float>(id, j->second.first->getFrequency()));

				max_tasks = ordering > max_tasks ? ordering : max_tasks; 
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
		Scalar<float> ampl_M("M", getRoundTime());

		algebra.insert(&ampl_M);
		algebra.insert(&ampl_schedule);
		algebra.insert(&ampl_frequencies);

		return algebra;
	}

	double
	Schedule::getRoundTime() const
	{
		return roundTime;
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
	
	const Taskgraph&
	Schedule::getTaskgraph() const
	{
		return this->taskgraph;
	}

	Schedule&
	Schedule::operator=(const Schedule &copy)
	{
		this->name = name;
		this->appName = appName;
		roundTime = roundTime;
	
		// Copy taskgraph	
		this->taskgraph = taskgraph;
		this->setSchedule(schedule);

		return *this;
	}

	const set<const Task*>&
	Schedule::getTasks(int core, std::map<int, std::set<const Task*> >& tasks) const
	{
		if(tasks.find(core) == tasks.end())
		{
			set<const Task*> core_tasks;
			for(sequence::const_iterator i = this->schedule.find(core)->second.begin(); i != this->schedule.find(core)->second.end(); i++)
			{
				core_tasks.insert(i->second.first);
			}

			tasks.insert(pair<int, set<const Task*> >(core, core_tasks));
		}

		return tasks.find(core)->second;
	}

	const set<const Task*>&
	Schedule::getProducers(int core, std::map<int, set<const Task*> > &producers) const
	{
		map<int, set<const Task*> > tasks;
		this->getTasks(core, tasks);
		
		if(producers.find(core) == producers.end())
		{
			set<const Task*> prods;
			for(set<const Task*>::const_iterator i = this->getTasks(core, tasks).begin(); i != this->getTasks(core, tasks).end(); i++)
			{
				for(set<const Link*>::const_iterator j = (*i)->getProducers().begin(); j != (*i)->getProducers().end(); j++)
				{
					if(this->getTasks(core, tasks).find((*j)->getProducer()) == this->getTasks(core, tasks).end())
					{
						prods.insert((*j)->getProducer());
					}
				}
			}
	
			producers.insert(pair<int, set<const Task*> >(core, prods));
		}

		return producers.find(core)->second;
	}

	const set<const Task*>&
	Schedule::getConsumers(int core, std::map<int, set<const Task*> > &consumers) const
	{
		map<int, set<const Task*> > tasks;
		this->getTasks(core, tasks);

		if(consumers.find(core) == consumers.end())
		{
			set<const Task*> prods;
			for(set<const Task*>::const_iterator i = this->getTasks(core, tasks).begin(); i != this->getTasks(core, tasks).end(); i++)
			{
				for(set<const Link*>::const_iterator j = (*i)->getConsumers().begin(); j != (*i)->getConsumers().end(); j++)
				{
					if(this->getTasks(core, tasks).find((*j)->getConsumer()) == this->getTasks(core, tasks).end())
					{
						prods.insert((*j)->getConsumer());
					}
				}
			}
	
			consumers.insert(pair<int, set<const Task*> >(core, prods));
		}

		return consumers.find(core)->second;
	}

	const set<const Task*>&
	Schedule::getProducers(int core)
	{
		return this->getProducers(core, this->producers);
	}

	const set<const Task*>&
	Schedule::getConsumers(int core)
	{
		return this->getConsumers(core, this->consumers);
	}

	const set<const Task*>&
	Schedule::getTasks(int core)
	{
		return this->getTasks(core, this->tasks);
	}

	const set<int>
	Schedule::getCores(const Task* t) const
	{
		set<int> cores;
		for(table::const_iterator i = this->getSchedule().begin(); i != this->getSchedule().end(); i++)
		{
			for(sequence::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				if(j->second.first == t)
				{
					cores.insert(i->first);
				}
			}
		}

		return cores;
	}
}
