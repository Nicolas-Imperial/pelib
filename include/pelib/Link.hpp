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
