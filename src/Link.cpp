/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <string.h>

#include <pelib/Link.hpp>
#include <pelib/PelibException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Link::Link(const Task &producer, const Task &consumer, const std::string &producerName, const std::string &consumerName, const Buffer &queue_buffer, const Buffer &producer_buffer, const Buffer &consumer_buffer, size_t producer_rate, size_t consumer_rate): queueBuffer(queue_buffer), producerBuffer(producer_buffer), consumerBuffer(consumer_buffer)
	{
		this->producerName = producerName;
		this->consumerName = consumerName;
		this->producer = (Task*)&producer;
		this->consumer = (Task*)&consumer;
		this->producer_rate = producer_rate;
		this->consumer_rate = consumer_rate;
		this->queueBuffer = queue_buffer;
		this->producerBuffer = producer_buffer;
		this->consumerBuffer = consumer_buffer;
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

	std::string
	Link::getDataType() const
	{
		return this->queueBuffer.getType();
	}

	void
	Link::setProducer(Task* producer)
	{
		this->producer = producer;
	}

	void
	Link::setConsumer(Task* consumer)
	{
		this->consumer = consumer;
	}

	size_t
	Link::getProducerRate() const
	{
		return this->producer_rate;
	}

	size_t
	Link::getConsumerRate() const
	{
		return this->consumer_rate;
	}

	std::string
	Link::getProducerName() const
	{
		return this->producerName;
	}

	std::string
	Link::getConsumerName() const
	{
		return this->consumerName;
	}

	const Buffer&
	Link::getQueueBuffer() const
	{
		return this->queueBuffer;
	}

	const Buffer&
	Link::getProducerBuffer() const
	{
		return this->producerBuffer;
	}

	const Buffer&
	Link::getConsumerBuffer() const
	{
		return this->consumerBuffer;
	}

	bool
	Link::operator<(const Link &other) const
	{
		Task thisProducer = *this->getProducer();
		Task otherProducer = *other.getProducer();
		Task thisConsumer = *this->getConsumer();
		Task otherConsumer = *other.getConsumer();

		if(thisProducer == otherProducer && thisConsumer == otherConsumer)
		{
			if(this->getProducerName().compare(other.getProducerName()) == 0)
			{
				return this->getConsumerName() < other.getConsumerName();
			}
			else
			{
				return this->getProducerName() < other.getProducerName();
			}
		}

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
		return (*this->getProducer() == *other.getProducer()) && (*this->getConsumer() == *other.getConsumer()) && (this->getProducerName().compare(other.getProducerName()) == 0) && (this->getConsumerName().compare(other.getConsumerName()) == 0);
	}
}
