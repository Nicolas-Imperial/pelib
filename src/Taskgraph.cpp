#include <Taskgraph.hpp>
#include <MakespanCalculator.hpp>
#include <ParseException.hpp>

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>

namespace pelib
{
	Taskgraph::Taskgraph()
	{
	}

	Taskgraph::Taskgraph(const Taskgraph* graph)
	{
	}

	Taskgraph::~Taskgraph()
	{
	}

	Taskgraph*
	Taskgraph::clone() const
	{
		return new Taskgraph();
	}

	Algebra
	Taskgraph::buildAlgebra() const
	{
		return buildAlgebra(Architecture());
	}
	
	Algebra
	Taskgraph::buildAlgebra(const Architecture &arch) const
	{
		Algebra out;

		Scalar<int> n("n", getTasks().size());
		map<int, map<int, float> > map_e;
		map<int, int> map_tau;
		map<int, int> map_Wi;

		for(set<Task>::const_iterator i = getTasks().begin(); i != getTasks().end(); i++)
		{
			map_tau.insert(pair<int, int>(i->getId(), i->getWorkload()));
			map_Wi.insert(pair<int, int>(i->getId(), i->getMaxWidth()));

			map<int, float> task_e;
			for(int j = 1; j <= arch.getCoreNumber(); j++)
			{
				task_e.insert(pair<int, float>(j, i->getEfficiency(j)));
			}
			
			map_e.insert(pair<int, map<int, float> >(i->getId(), task_e));
		}

		Vector<int, int> tau("tau", map_tau);
		Vector<int, int> Wi("Wi", map_Wi);
		Matrix<int, int, float> e("e", map_e);

		out.insert(&tau);
		out.insert(&Wi);
		out.insert(&e);

		return out;
	}

	string
	Taskgraph::getAUTName() const
	{
		return this->autName;
	}

	void
	Taskgraph::setAUTName(const string autName)
	{
		this->autName = autName;
	}

	string
	Taskgraph::getMakespanCalculator() const
	{
		return this->makespanCalculator;
	}

	void
	Taskgraph::setMakespanCalculator(const string makespanCalculator)
	{
		try{
			MakespanCalculator *calculator = MakespanCalculator::getMakespanCalculator(makespanCalculator);
			delete calculator;
		} catch (ParseException &e)
		{
			throw e;
		}
		
		this->makespanCalculator = makespanCalculator;
	}

	double
	Taskgraph::getRoundTime(Architecture &arch) const
	{
		MakespanCalculator *calculator = MakespanCalculator::getMakespanCalculator(this->getMakespanCalculator());
		calculator->calculate(*this, arch);
		delete calculator;
		
		return 0;
	}
	
	const set<Task>&
	Taskgraph::getTasks() const
	{
		return tasks;
	}

	void
	Taskgraph::setTasks(const set<Task>& tasks)
	{
		set<string> task_ids;

		for(set<Task>::const_iterator iter = tasks.begin(); iter != tasks.end(); iter++)
		{
			task_ids.insert(iter->getTaskId());
		}

		if(task_ids.size() != tasks.size())
		{
			throw ParseException("The tasks added do not have a unique taskId.");
		}
		
		this->tasks = tasks;
	}

	const Task&
	Taskgraph::findTask(const string taskId) const
	{
		for(set<Task>::const_iterator iter = this->tasks.begin(); iter != this->tasks.end(); iter++)
		{
			if(strcmp(iter->getTaskId().c_str(), taskId.c_str()) == 0)
			{
				return *iter;
			}
		}

		throw ParseException("No task \"" + taskId + "\" exists in this taskgraph.");
	}
}
