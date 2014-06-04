#include <Taskgraph.hpp>
#include <MakespanCalculator.hpp>
#include <ParseException.hpp>
#include <CastException.hpp>

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>

using namespace std;

namespace pelib
{
	Taskgraph::Taskgraph() {}

	Taskgraph::Taskgraph(const Taskgraph *graph)
	{
		this->autName = graph->getAUTName();
		this->makespanCalculator = graph->getMakespanCalculator();
		this->tasks = graph->getTasks();
	}

	Taskgraph::Taskgraph(const Algebra &algebra)
	{
		this->autName = "Generated_from_algebra";
		const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Scalar<int> *n = algebra.find<Scalar<int> >("n");
		const Vector<int, int> *tau = algebra.find<Vector<int, int> >("Tau");
		const Vector<int, int> *Wi = algebra.find<Vector<int, int> >("Wi");
		const Matrix<int, int, float> *e = algebra.find<Matrix<int, int, float> >("e");
		
		if(M == NULL || n == NULL || tau == NULL || Wi == NULL || e == NULL)
		{
			throw CastException("Missing parameter. Need float scalar M, integer scalar n, integer vectors tau and Wi (of size n), and float matrix e of n lines.");
		}
		else
		{
			stringstream ss;
			ss << M->getValue();
			this->makespanCalculator = ss.str(); 
		}

		for(map<int, int>::const_iterator i = tau->getValues().begin(); i != tau->getValues().end(); i++)
		{
			int id = i->first;
			int work = i->second;
			int max_wi = Wi->getValues().find(id)->second;

			stringstream ss;
			for(map<int, float>::const_iterator j = e->getValues().find(id)->second.begin(); j != e->getValues().find(id)->second.end(); j++)
			{
				ss << j->second << " ";
			}
			
			Task t(id);
			t.setWorkload(work);
			t.setMaxWidth(max_wi);
			t.setEfficiencyString(ss.str());

			this->tasks.insert(t);
		}
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

		Vector<int, int> tau("Tau", map_tau);
		Vector<int, int> Wi("Wi", map_Wi);
		Matrix<int, int, float> e("e", map_e);
		Scalar<float> M("M", getRoundTime(arch));

		out.insert(&n);
		out.insert(&tau);
		out.insert(&Wi);
		out.insert(&e);
		out.insert(&M);

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
	Taskgraph::getRoundTime(const Architecture &arch) const
	{
		//cerr << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] getTasks().size() = " << getTasks().size() << endl;
		MakespanCalculator *calculator = MakespanCalculator::getMakespanCalculator(this->getMakespanCalculator());
		double time = calculator->calculate(*this, arch);
		delete calculator;
		
		return time;
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
