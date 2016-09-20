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

#include <pelib/Task.hpp>

#ifndef PELIB_LINK
#define PELIB_LINK

namespace pelib
{
	/** Model a link between tasks of a streaming application **/
	class Link
	{
		public:
			/** Constructor. Takes the producer and consumer tasks at both ends of the link **/
			Link(const Task &producer, const Task &consumer, const std::string &producerName, const std::string &consumerName, const std::string &type = "", size_t rate = 0);

			/** Returns a pointer to the producer task **/
			virtual Task* getProducer() const;

			/** Returns a pointer to the consumer task **/
			virtual Task* getConsumer() const;

			/** Check if this link is striclty inferior to another link given 
				@param other Other link to compare this instance to
			**/
			virtual bool
			operator<(const Link &other) const;

			/** Returns true if this instance is considered equivalent to another link
				@param other Other link to be compared
			**/
	    		virtual bool
			operator==(const Link &other) const;

			size_t
			getRate() const;

			std::string
			getDataType() const;

			std::string
			getProducerName() const;

			std::string
			getConsumerName() const;
		protected:
			/** Producer and consumer task pointers **/
			Task *producer, *consumer;
			std::string producerName, consumerName, type;
			size_t rate;

		private:		
	};
}

#endif
