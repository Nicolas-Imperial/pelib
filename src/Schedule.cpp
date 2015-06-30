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
		this->name = name;
		this->appName = appName;
	}

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
		this->schedule = schedule;
		this->tasks.clear();
		this->core_tasks.clear();

		for(table::iterator i = this->schedule.begin(); i != this->schedule.end(); i++)
		{
			set<const Task*> this_core_tasks;
			for(sequence::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				const Task *task = j->second.first;
				Task t = *task;
				this->tasks.insert(t);
				Task &task_ref = (Task&)*this->getTasks().find(t);
				pair<float, work> new_pair = pair<float, work>(j->first, work(&task_ref, j->second.second));
				i->second.erase(j);
				i->second.insert(new_pair);

				// Insert a reference to this task in the core's task list
				this_core_tasks.insert(&t);
			}
			this->core_tasks.insert(pair<int, set<const Task*> >(i->first, this_core_tasks));
		}
	}

#define debug(expr) cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << expr << "\"." << endl;
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
		/*
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
		*/
	}

	const set<const Task*>&
	Schedule::getProducers(int core, const pelib::Taskgraph &tg) const
	{
		/*
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
		*/
	}

	const set<const Task*>&
	Schedule::getConsumers(int core, const pelib::Taskgraph &tg) const
	{
		/*
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
		*/
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
