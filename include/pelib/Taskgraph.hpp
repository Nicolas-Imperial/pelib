#include <set>
#include <string>

#include <pelib/Algebra.hpp>
#include <pelib/Task.hpp>
#include <pelib/Link.hpp>
#include <pelib/Platform.hpp>

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

namespace pelib
{
	using namespace std;
	
	class Taskgraph: public Record
	{
		public:
			Taskgraph();
			Taskgraph(const Taskgraph *graph);
			Taskgraph(const Taskgraph &graph);
			Taskgraph(const set<Task>&, const set<Link>&);
			Taskgraph(const Algebra &algebra);
			virtual ~Taskgraph();

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
			getDeadlineCalculator() const;

			virtual double
			getDeadline(const Platform &arch) const;

			virtual void
			setDeadlineCalculator(const string deadlineCalculator);

			virtual const set<Task>&
			getTasks() const;

			virtual set<Task>&
			getTasks();

			//virtual void
			//setTasks(const set<Task>&);

			virtual const Task&
			findTask(const string &taskId) const;

			virtual const set<Link>&
			getLinks() const;

			virtual set<Link>&
			getLinks();

			//virtual void
			//setLinks(const set<Link>&);

			virtual Taskgraph&
			operator=(const Taskgraph& copy);

		protected:
			string name, deadlineCalculator;
			set<Task> tasks;
			set<Link> links;
			
			virtual void
			setLinks(const set<Link> &link);
		private:		
	};
}

#endif
