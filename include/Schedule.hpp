#include <StreamingAppData.hpp>
#include <Algebra.hpp>
#include <Task.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{
	class Schedule : public StreamingAppData
	{
		public:
			Schedule(std::string name, std::string autName);
			Schedule(Algebra &algebra);
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
			getAUTName() const;

			virtual void
			setAUTName(const std::string autName);

			virtual const std::map<int, std::vector<Task> >&
			getSchedule() const;

			virtual void
			setSchedule(const std::map<int, std::vector<Task> >& schedule);

		protected:
			double roundTime;
			std::string name, autName;
			std::map<int, std::vector<Task> > schedule;
			
		private:		
	};
}

#endif
