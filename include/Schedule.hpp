#include <Algebra.hpp>
#include <Link.hpp>
#include <Task.hpp>
#include <Taskgraph.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{	
	class Schedule : public Record
	{
		public:
			typedef std::pair<const Task*, double> work;
			typedef std::map<float, work> sequence;
			typedef std::map<int, sequence> table;

			Schedule(const std::string &name, const std::string &appName, const table &schedule);
			Schedule(const std::string &name, const Algebra &algebra);
			virtual ~Schedule();
			virtual Schedule* clone() const;
			virtual Algebra buildAlgebra() const;

			//virtual void
			//setMakespan(double makespan);

			virtual std::string
			getName() const;

			//virtual void
			//setName(const std::string name);

			virtual std::string
			getAppName() const;

			//virtual void
			//setAppName(const std::string name);

			virtual const table&
			getSchedule() const;

			virtual const set<Task>&
			getTasks() const;

			virtual Schedule&
			operator=(const Schedule &);

			virtual const set<const Task*>&
			getTasks(int core) const;

			virtual set<const Task*>
			getProducers(int core, const Taskgraph &tg) const;

			virtual set<const Task*>
			getConsumers(int core, const Taskgraph &tg) const;

			virtual const set<int>
			getCores(const Task*) const;

		protected:
			std::string name, appName;
			table schedule;
			set<Task> tasks;
			std::map<int, set<const Task*> > core_tasks;
			
			virtual void
			setSchedule(const table&);
		private:		
	};
}

#endif
