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

#include <pelib/Algebra.hpp>
#include <pelib/Link.hpp>
#include <pelib/Task.hpp>
#include <pelib/Taskgraph.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{	
	class Schedule : public Record
	{
		public:
			typedef std::pair<const Task*, double> work;
			typedef std::map<float, work> sequence;
			typedef std::map<int, sequence> table;

			Schedule(const std::string &name, const std::string &appName, const table &schedule);
			Schedule(const std::string &name, const Algebra &algebra);
			virtual ~Schedule();
			virtual Schedule* clone() const;
			virtual Algebra buildAlgebra() const;

			virtual std::string
			getName() const;

			virtual std::string
			getAppName() const;

			virtual const table&
			getSchedule() const;

			virtual const set<Task>&
			getTasks() const;

			virtual Schedule&
			operator=(const Schedule &);

			virtual const set<const Task*>&
			getTasks(int core) const;

			virtual const Task&
			getTask(int id) const;

			virtual set<const Task*>
			getTasksSharedMemoryIsland(const set<int>& islands, const Platform &pt) const;

			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandProducers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const;

			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandTaskProducers(const Task &t, const Taskgraph &tg, const Platform &pt) const;

			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandConsumers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const;

			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandTaskConsumers(const Task &t, const Taskgraph &tg, const Platform &pt) const;

			virtual const set<int>
			getCores(const Task&) const;

			static
			Algebra addStartTime(const Algebra &data, const Taskgraph &tg, const Platform &platform);

		protected:
			std::string name, appName;
			table schedule;
			set<Task> tasks;
			std::map<int, set<const Task*> > core_tasks;
			
			virtual void
			setSchedule(const table&);
		private:		
	};
}

#endif
