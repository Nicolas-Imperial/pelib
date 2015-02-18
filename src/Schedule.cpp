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

namespace pelib
{
	Schedule::Schedule(const std::string &name, const std::string &autName)
	{
		roundTime = 0;
		this->name = name;
		this->autName = autName;
	}

	Schedule::Schedule(const std::string &name, const Taskgraph &tg, const Algebra &data)
	{
		roundTime = 0;
		this->name = name;
		this->autName = tg.getAUTName();

		Algebra algebra = data.merge(tg.buildAlgebra());
		
		const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Vector<int, float> *tau = algebra.find<Vector<int, float> >("Tau");
		const Vector<int, float> *starting_time = algebra.find<Vector<int, float> >("starting_time");
		const Vector<int, float> *wi = algebra.find<Vector<int, float> >("wi");
		const Matrix<int, int, float> *sched = algebra.find<Matrix<int, int, float> >("schedule");
		const Vector<int, float> *freq = algebra.find<Vector<int, float> >("frequency");
		
		if(M == NULL || tau == NULL || wi == NULL || sched == NULL || freq == NULL)
		{
			throw CastException("Missing parameter");
		}
		else
		{
			this->roundTime = M->getValue();

			for(map<int, map<int, float> >::const_iterator i = sched->getValues().begin(); i != sched->getValues().end(); i++)
			{
				map<float, Task> core_schedule;
				float start = 0;
				
				for(map<int, float>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					if(floor(j->second) > 0)
					{
						Task task((int)j->second, tg.findTask((int)floor(j->second)).getTaskId());
						task.setWorkload(tau->getValues().find((int)floor(j->second))->second);
						if(task.getWorkload() > 0)
						{
							task.setWidth(wi->getValues().find((int)floor(j->second))->second);
							task.setFrequency(freq->getValues().find((int)floor(j->second))->second);
							task.setMaxWidth(tg.getTasks().find(task)->getMaxWidth());
							task.setEfficiencyString(tg.getTasks().find(task)->getEfficiencyString());

							if(starting_time == NULL)
							{
								task.setStartTime(start);
							}
							else
							{
								task.setStartTime(starting_time->getValues().find((int)j->second)->second);
							}

							//cerr << "Core: " << i->first << "; Task: " << j->second << "; taskid: " << tg.findTask((int)floor(j->second)).getTaskId() << "; Start time: " << task.getStartTime() << endl;
							core_schedule.insert(pair<float, Task>(task.getStartTime(), task));
							start += task.runtime(task.getWidth(), task.getFrequency());
						}
					}
				}

				schedule.insert(pair<int, map<float, Task> >(i->first, core_schedule));
			}
		}
	}
	
	Schedule*
	Schedule::clone() const
	{
		Schedule *clone = new Schedule(getName(), getAUTName());
		clone->setRoundTime(getRoundTime());
		clone->setSchedule(getSchedule());

		return clone;
	}

	Algebra
	Schedule::buildAlgebra() const
	{
		Algebra algebra;
		
		map<string, int>taskid2id; 
		for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				taskid2id.insert(pair<string, int>(j->second.getTaskId(), j->second.getId()));
			}
		}

		map<int, map<int, float> > sched;
		map<int, float> frequencies;
		const map<int, map<float, Task> > schedule = getSchedule();

		size_t ordering, max_tasks = 0;
		for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			map<int, float> schedule_row;
			int core = i->first;
	
			ordering = 0;
			for(map<float, Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++, ordering++)
			{
				int id = taskid2id.find(j->second.getTaskId())->second;
				schedule_row.insert(pair<int, int>(ordering, id));
				frequencies.insert(pair<int, float>(id, j->second.getFrequency()));

				max_tasks = ordering > max_tasks ? ordering : max_tasks; 
			}

			sched.insert(pair<int, map<int, float> >(core + 1, schedule_row));
		}

		// pad with 0
		for(map<int, map<float, Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
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

	void
	Schedule::setRoundTime(double roundTime)
	{
		this->roundTime = roundTime;
	}

	std::string
	Schedule::getName() const
	{
		return this->name;
	}

	void
	Schedule::setName(const std::string name)
	{
		this->name = name;
	}

	std::string
	Schedule::getAUTName() const
	{
		return this->autName;
	}

	void
	Schedule::setAUTName(const std::string autName)
	{
		this->autName = autName;
	}

	const std::map<int, std::map<float, Task> >&
	Schedule::getSchedule() const
	{
		return schedule;
	}
	
	void
	Schedule::setSchedule(const std::map<int, std::map<float, Task> > &schedule)
	{
		this->schedule = schedule;
	}
}
