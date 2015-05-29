#include <StreamingAppData.hpp>
#include <Algebra.hpp>
#include <Link.hpp>
#include <Task.hpp>
#include <Taskgraph.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{	
	class Schedule : public StreamingAppData
	{
		public:
			typedef std::pair<const Task*, double> work;
			typedef std::map<float, work> sequence;
			typedef std::map<int, sequence> table;

			Schedule(const std::string &name, const std::string &appName);
			Schedule(const std::string &name, const std::string &appName, const Taskgraph &taskgraph, const table &schedule, double roundTime);
			Schedule(const std::string &name, const Taskgraph &tg, const Algebra &algebra);
			virtual Schedule* clone() const;
			virtual Algebra buildAlgebra() const;

			virtual double
			getRoundTime() const;

			//virtual void
			//setRoundTime(double roundTime);

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

			virtual const Taskgraph&
			getTaskgraph() const;

			virtual Schedule&
			operator=(const Schedule &);

			virtual const set<const Task*>&
			getProducers(int core);

			virtual const set<const Task*>&
			getConsumers(int core);

			virtual const set<const Task*>&
			getTasks(int core);

			virtual const set<const Task*>&
			getProducers(int core, std::map<int, std::set<const Task*> >&) const;

			virtual const set<const Task*>&
			getConsumers(int core, std::map<int, std::set<const Task*> >&) const;

			virtual const set<const Task*>&
			getTasks(int core, std::map<int, std::set<const Task*> >&) const;

			virtual const set<int>
			getCores(const Task*) const;

		protected:
			double roundTime;
			std::string name, appName;
			table schedule;
			Taskgraph taskgraph;
			std::map<int, set<const Task*> > producers;
			std::map<int, set<const Task*> > consumers;
			std::map<int, set<const Task*> > tasks;
			
			virtual void
			setSchedule(const table&);
		private:		
	};
}

#endif
