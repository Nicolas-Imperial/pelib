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


#include <set>
#include <string>

#include <pelib/Algebra.hpp>
#include <pelib/Task.hpp>
#include <pelib/Link.hpp>
#include <pelib/Platform.hpp>

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

namespace pelib
{
	using namespace std;
	
	class Taskgraph: public Record
	{
		public:
			Taskgraph();
			Taskgraph(const Taskgraph *graph);
			Taskgraph(const Taskgraph &graph);
			Taskgraph(const set<Task>&, const set<Link>&);
			Taskgraph(const Algebra &algebra);
			virtual ~Taskgraph();

			virtual
			Taskgraph*
			clone() const;

			virtual Algebra buildAlgebra() const;
			virtual Algebra buildAlgebra(const Platform &arch) const;

			virtual string
			getName() const;

			virtual void
			setName(const string name);

			virtual string
			getDeadlineCalculator() const;

			virtual double
			getDeadline(const Platform &arch) const;

			virtual void
			setDeadlineCalculator(const string deadlineCalculator);

			virtual const set<Task>&
			getTasks() const;

			virtual set<Task>&
			getTasks();

			//virtual void
			//setTasks(const set<Task>&);

			virtual const Task&
			findTask(const string &taskId) const;

			virtual const set<Link>&
			getLinks() const;

			virtual set<Link>&
			getLinks();

			//virtual void
			//setLinks(const set<Link>&);

			virtual Taskgraph&
			operator=(const Taskgraph& copy);

		protected:
			string name, deadlineCalculator;
			set<Task> tasks;
			set<Link> links;
			
			virtual void
			setLinks(const set<Link> &link);
		private:		
	};
}

#endif
