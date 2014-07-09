#include <set>
#include <string>

#include <Schedule.hpp>
#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <CastException.hpp>

using namespace std;

namespace pelib
{
	Schedule::Schedule(std::string name, std::string autName)
	{
		roundTime = 0;
		this->name = name;
		this->autName = autName;
	}

	Schedule::Schedule(std::string name, std::string autName, Algebra &algebra)
	{
		roundTime = 0;
		this->name = name;
		this->autName = autName;

		this->autName = "Generated_from_algebra";
		const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Vector<int, int> *tau = algebra.find<Vector<int, int> >("Tau");
		const Vector<int, int> *wi = algebra.find<Vector<int, int> >("wi");
		const Matrix<int, int, int> *sched = algebra.find<Matrix<int, int, int> >("schedule");
		const Vector<int, int> *freq = algebra.find<Vector<int, int> >("frequency");
		
		if(M == NULL || tau == NULL || wi == NULL || sched == NULL || freq == NULL)
		{
			throw CastException("Missing parameter");
		}
		else
		{
			this->roundTime = M->getValue();

			for(map<int, map<int, int> >::const_iterator i = sched->getValues().begin(); i != sched->getValues().end(); i++)
			{
				vector<Task> core_schedule;
				
				for(map<int, int>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
				{
					if(j->second > 0)
					{
						Task task(j->second);
						task.setWorkload(tau->getValues().find(j->second)->second);
						task.setWidth(wi->getValues().find(j->second)->second);
						task.setFrequency(freq->getValues().find(j->second)->second);

						core_schedule.push_back(task);
					}
				}

				schedule.insert(pair<int, vector<Task> >(i->first, core_schedule));
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

		std::set<std::string> task_ids;
		for(map<int, vector<Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			for(vector<Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				task_ids.insert(j->getTaskId());
			}
		}

		map<string,int>taskid2id;
		int counter = 1;
		for(set<string>::const_iterator i = task_ids.begin(); i != task_ids.end(); i++, counter++)
		{
			taskid2id.insert(pair<string, int>(*i, counter));
		}

		map<int, map<int, int> > sched;
		map<int,int> frequencies;
		const map<int, vector<Task> > schedule = getSchedule();
		size_t ordering = 0, max_tasks = 0;
		for(map<int, vector<Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			map<int, int> schedule_row;
			int core = i->first;			

			ordering = 0;
			for(vector<Task>::const_iterator j = i->second.begin(); j != i->second.end(); j++, ordering++)
			{
				int id = taskid2id.find(j->getTaskId())->second;
				schedule_row.insert(pair<int, int>(ordering, id));
				frequencies.insert(pair<int, int>(id, j->getFrequency()));

				max_tasks = ordering > max_tasks ? ordering : max_tasks; 
			}

			sched.insert(pair<int, map<int,int> >(core+1,schedule_row));
		}

		// pad with 0
		for(map<int, vector<Task> >::const_iterator i = schedule.begin(); i != schedule.end(); i++)
		{
			size_t base = i->second.size();
			for(ordering = base; ordering < max_tasks; ordering++)
			{
				sched.find(i->first)->second.insert(pair<int, int>(ordering, 0));
			}
		}
		
		Matrix<int,int,int> ampl_schedule("schedule", sched);
		Vector<int,int> ampl_frequencies("frequency", frequencies);
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

	const std::map<int, std::vector<Task> >&
	Schedule::getSchedule() const
	{
		return this->schedule;
	}

	void
	Schedule::setSchedule(const std::map<int, std::vector<Task> >& schedule)
	{
		this->schedule = schedule;
	}
}
