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
