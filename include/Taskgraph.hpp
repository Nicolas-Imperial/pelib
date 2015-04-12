#include <set>
#include <string>

#include <StreamingAppData.hpp>
#include <Algebra.hpp>
#include <Task.hpp>
#include <Platform.hpp>

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

namespace pelib
{
	using namespace std;
	
	class Taskgraph: public StreamingAppData
	{
		public:
			Taskgraph();
			Taskgraph(const Taskgraph *graph);
			Taskgraph(const Algebra &algebra);

			virtual
			Taskgraph*
			clone() const;

			virtual Algebra buildAlgebra() const;
			virtual Algebra buildAlgebra(const Platform &arch) const;

			virtual string
			getName() const;

			virtual void
			setName(const string name);

			virtual string
			getMakespanCalculator() const;

			virtual double
			getRoundTime(const Platform &arch) const;

			virtual void
			setMakespanCalculator(const string makespanCalculator);

			virtual const set<Task>&
			getTasks() const;

			virtual set<Task>&
			getTasks();

			virtual void
			setTasks(const set<Task>&);

			virtual const Task&
			findTask(const string &taskId) const;

			virtual const Task&
			findTask(int id) const;
				
		protected:
			string name, makespanCalculator;
			set<Task> tasks;
			
		private:		
	};
}

#endif
