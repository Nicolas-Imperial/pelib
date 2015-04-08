#include <StreamingAppData.hpp>
#include <Algebra.hpp>
#include <Taskgraph.hpp>
#include <Task.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{	
	class Schedule : public StreamingAppData
	{
		public:
			Schedule(const std::string &name, const std::string &autName);
			Schedule(const std::string &name, const Taskgraph &tg, const Algebra &algebra);
			virtual Schedule* clone() const;
			virtual Algebra buildAlgebra() const;

			virtual double
			getRoundTime() const;

			virtual void
			setRoundTime(double roundTime);

			virtual std::string
			getName() const;

			virtual void
			setName(const std::string name);

			virtual std::string
			getAppName() const;

			virtual void
			setAppName(const std::string autName);

			virtual const std::map<int, std::map<float, Task> >&
			getSchedule() const;

			virtual void
			setSchedule(const std::map<int, std::map<float, Task> > &schedule);

		protected:
			double roundTime;
			std::string name, appName;
			std::map<int, std::map<float, Task> > schedule;
			
		private:		
	};
}

#endif
