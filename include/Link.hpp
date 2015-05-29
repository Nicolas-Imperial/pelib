#include <Task.hpp>

#ifndef PELIB_LINK
#define PELIB_LINK

namespace pelib
{
	class Link
	{
		public:
			Link(const Task &producer, const Task &consumer);
			virtual Task* getProducer() const;
			virtual Task* getConsumer() const;

			virtual bool
			operator<(const Link &other) const;

	    		virtual bool
			operator==(const Link &other) const;

		protected:
			Task *producer, *consumer;

		private:		
	};
}

#endif
