#include <string>

#ifndef PELIB_TASK
#define PELIB_TASK

namespace pelib
{
	class Task
	{
		public:
			Task(int id, std::string taskId);
			
			virtual double
			getFrequency() const;

			virtual void
			setFrequency(double frequency);

			virtual double
			getWidth() const;

			virtual void
			setWidth(double width);

			virtual std::string
			getName() const;

			virtual void
			setName(const std::string name);

			virtual std::string
			getTaskId() const;

			virtual int
			getId() const;

			virtual std::string
			getEfficiencyString() const;

			virtual void
			setEfficiencyString(const std::string efficiencyString);

			virtual double
			getEfficiency(int p, double def = very_small) const;
			
			virtual double
			getWorkload() const;

			virtual void
			setWorkload(double workload);
			
			virtual double
			getMaxWidth() const;

			virtual void
			setMaxWidth(double maxWidth);

			virtual bool
			runtime(double width = 1, double frequency = 1) const;

			virtual bool
			operator<(const Task &other) const;
    
    		virtual bool
			operator==(const Task &other) const;
			
		protected:
			double frequency, width;
			double workload, maxWidth;
			int id;
			std::string taskId, name, efficiencyString;
			
			static constexpr float very_small = 1e-6;
			
		private:
	};
}

			

#endif
