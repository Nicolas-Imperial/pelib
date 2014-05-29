#include <string>

#ifndef PELIB_TASK
#define PELIB_TASK

namespace pelib
{
	class Task
	{
		public:
			Task(std::string id);
			
			virtual int
			getFrequency() const;

			virtual void
			setFrequency(int frequency);

			virtual std::string
			getId() const;
			
		protected:
			int frequency;
			std::string id;
		private:		
	};
}

#endif
