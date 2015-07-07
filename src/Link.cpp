#include <pelib/Link.hpp>

namespace pelib
{
	Link::Link(const Task &producer, const Task &consumer)
	{
		this->producer = (Task*)&producer;
		this->consumer = (Task*)&consumer;
	}

	Task*
	Link::getProducer() const
	{
		return this->producer;
	}

	Task*
	Link::getConsumer() const
	{
		return this->consumer;
	}

	bool
	Link::operator<(const Link &other) const
	{
		Task thisProducer = *this->getProducer();
		Task otherProducer = *other.getProducer();
		Task thisConsumer = *this->getConsumer();
		Task otherConsumer = *other.getConsumer();

		if(thisProducer == otherProducer)
		{
			return thisConsumer < otherConsumer;
		}
		else
		{
			return thisProducer < otherProducer;
		}	
	}

	bool
	Link::operator==(const Link &other) const
	{	
		return (*this->getProducer() == *other.getProducer()) && (*this->getConsumer() == *other.getConsumer());
	}
}
