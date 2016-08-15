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

#include <pelib/Link.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	Link::Link(const Task &producer, const Task &consumer, const string &producerName, const string &consumerName, const std::string &type, size_t consume, size_t produce)
	{
		this->producerName = producerName;
		this->consumerName = consumerName;
		this->producer = (Task*)&producer;
		this->consumer = (Task*)&consumer;
		this->type = string(type);
		this->consume = consume;
		this->produce = produce;
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

	size_t
	Link::getConsumerRate() const
	{
		return this->consume;
	}

	size_t
	Link::getProducerRate() const
	{
		return this->produce;
	}

	std::string
	Link::getDataType() const
	{
		return this->type;
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
