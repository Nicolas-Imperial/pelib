#include <sstream>

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
	Taskgraph::Taskgraph()
	{
	}

	Taskgraph::Taskgraph(const set<Task> &tasks, const set<Link> &links)
	{
		// Check if all tasks have a unique string ID
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
		this->setLinks(links);
	}

	void
	Taskgraph::setLinks(const set<Link> &links)
	{
		// Add all links so their endpoints point to tasks in the local collection
		this->links.clear();
		for(set<Link>::const_iterator i = links.begin(); i != links.end(); i++)
		{
			const Task &producer = *this->tasks.find(*i->getProducer());
			const Task &consumer = *this->tasks.find(*i->getConsumer());

			Link link(producer, consumer);
			this->links.insert(link);
		}

		// Update the tasks' producer and consumer links
		for(set<Task>::iterator i = this->tasks.begin(); i != this->tasks.end(); i++)
		{
			Task &t = (Task&)*i;

			// Keep a copy of the link lists
			set<const Link*> producers = t.getProducers();
			set<const Link*> consumers = t.getConsumers();

			// Clear the link lists
			t.getProducers().clear();
			t.getConsumers().clear();
		
			// Reconstruct all links so they point to local tasks
			for(set<const Link*>::iterator j = producers.begin(); j != producers.end(); j++)
			{
				Task *producer = (*j)->getProducer();
				Task *consumer = (*j)->getConsumer();
				Link newLink(*this->tasks.find(*producer), *this->tasks.find(*consumer));
				const Link &link = *this->links.find(newLink);
				t.getProducers().insert(&link);
			}
			
			// Reconstruct all links so they point to local tasks
			for(set<const Link*>::iterator j = consumers.begin(); j != consumers.end(); j++)
			{
				Task *producer = (*j)->getProducer();
				Task *consumer = (*j)->getConsumer();
				Link newLink(*this->tasks.find(*producer), *this->tasks.find(*consumer));
				const Link &link = *this->links.find(newLink);
				t.getConsumers().insert(&link);
			}
		}
	}

	Taskgraph::Taskgraph(const Taskgraph *graph)
	{
		this->name = graph->getName();
		this->makespanCalculator = graph->getMakespanCalculator();
		this->tasks = graph->getTasks();
		this->setLinks(graph->getLinks());
	}

	Taskgraph::Taskgraph(const Taskgraph &graph)
	{
		this->name = graph.getName();
		this->makespanCalculator = graph.getMakespanCalculator();
		this->tasks = graph.getTasks();
		this->setLinks(graph.getLinks());
	}

	Taskgraph::Taskgraph(const Algebra &algebra)
	{
		this->name = "Generated_from_algebra";
		const Scalar<float> *M = algebra.find<Scalar<float> >("M");
		const Scalar<float> *n = algebra.find<Scalar<float> >("n");
		const Vector<int, float> *tau = algebra.find<Vector<int, float> >("Tau");
		const Vector<int, float> *Wi = algebra.find<Vector<int, float> >("Wi");
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

		for(map<int, float>::const_iterator i = tau->getValues().begin(); i != tau->getValues().end(); i++)
		{
			float id = i->first;
			float work = i->second;
			float max_wi = Wi->getValues().find((int)id)->second;

			stringstream ss;
			for(map<int, float>::const_iterator j = e->getValues().find((int)id)->second.begin(); j != e->getValues().find((int)id)->second.end(); j++)
			{
				ss << j->second << " ";
			}
			
			Task t((int)id);
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
		return buildAlgebra(Platform());
	}
	
	Algebra
	Taskgraph::buildAlgebra(const Platform &arch) const
	{
		Algebra out;

		Scalar<float> n("n", getTasks().size());
		map<int, map<int, float> > map_e;
		map<int, float> map_tau;
		map<int, float> map_Wi;

		for(set<Task>::const_iterator i = getTasks().begin(); i != getTasks().end(); i++)
		{
			map_tau.insert(pair<int, float>(i->getId(), i->getWorkload()));
			float max_width = 0;
			if(i->getMaxWidth() > arch.getCoreNumber())
			{
				max_width = arch.getCoreNumber();
			}
			else
			{
				max_width = i->getMaxWidth();
			}
			map_Wi.insert(pair<int, float>(i->getId(), max_width));

			map<int, float> task_e;
			for(int j = 1; j <= arch.getCoreNumber(); j++)
			{
				task_e.insert(pair<int, float>(j, i->getEfficiency(j)));
			}
			
			map_e.insert(pair<int, map<int, float> >(i->getId(), task_e));
		}

		Vector<int, float> tau("Tau", map_tau);
		Vector<int, float> Wi("Wi", map_Wi);
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
	Taskgraph::getName() const
	{
		return this->name;
	}

	void
	Taskgraph::setName(const string name)
	{
		this->name = name;
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
	Taskgraph::getRoundTime(const Platform &arch) const
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

	set<Task>&
	Taskgraph::getTasks()
	{
		return tasks;
	}

	/*
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
	*/

	const Task&
	Taskgraph::findTask(const string &taskId) const
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

	const Task&
	Taskgraph::findTask(int id) const
	{
		for(set<Task>::const_iterator iter = this->tasks.begin(); iter != this->tasks.end(); iter++)
		{
			if(iter->getId() == id)
			{
				return *iter;
			}
		}

		std::ostringstream stream;
		stream << "No task \"" << id << "\" exists in this taskgraph.";
		throw ParseException(stream.str());
	}

	const set<Link>&
	Taskgraph::getLinks() const
	{
		return this->links;
	}

	set<Link>&
	Taskgraph::getLinks()
	{
		return this->links;
	}

	Taskgraph&
	Taskgraph::operator=(const Taskgraph& copy)
	{
		this->name = copy.getName();
		this->makespanCalculator = copy.getMakespanCalculator();
		this->tasks = copy.getTasks();
		this->setLinks(copy.getLinks());	

		return *this;
	}

	/*
	void
	Taskgraph::setLinks(const set<Link>& links)
	{
		this->links = links;
	}
	*/
}
