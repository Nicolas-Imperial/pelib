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
#include <pelib/Platform.hpp>

#ifndef PELIB_SCHEDULE
#define PELIB_SCHEDULE

namespace pelib
{	
	/** Schedule of a streaming application **/
	class Schedule : public Record
	{
		public:
			/** Models the workload portion of a task computed in a portion of time of a schedule **/
			typedef std::pair<const Task*, double> work;
			/** Sequence of tasks to run for a processor **/
			typedef std::map<float, work> sequence;
			/** Collection to task sequences, one element per core **/
			typedef std::map<int, sequence> table;

			/** Constructor
				@param name name of the schedule. This is only for human-redability purpose
				@param appName name of the application this schedule is designed for
				@param schedule collection of task sequences to run by each processor in the schedule
			**/
			Schedule(const std::string &name, const std::string &appName, const table &schedule);
			/** Constructor 
				@param algebra Collection of AlgebraData descendant class instances that describes a schedule in an algebraic form, used as basis to build the Schedule class instance
			**/
			Schedule(const std::string &name, const Algebra &algebra);
			/** Constructor
				@param algebra Collection of AlgebraData descendant class instances that describes a schedule in an algebraic form, used as basis to build the Schedule class instance
			**/
			Schedule(const std::string &name, const std::string &appName, const Algebra &algebra);
			Schedule(const Schedule& src);

			/** Destructor **/
			virtual ~Schedule();
			/** Returns a pointer to a copy of this class instance **/
			virtual Schedule* clone() const;

			/** Generate an algebraic representation of this schedule **/
			virtual Algebra buildAlgebra() const;

			/** Returns the name of the schedule **/
			virtual std::string
			getName() const;

			/** Returns the name of the application this schedule is generated for **/
			virtual std::string
			getAppName() const;

			/** Returns a C++ standard collection that hold processors' task sequences **/
			virtual const table&
			getSchedule() const;

			/** Return the set of tasks scheduled in this class instance **/
			virtual const set<Task>&
			getTasks() const;

			/** Flushes this schedule instance and copies an instance of Schedule in this instance **/
			virtual Schedule&
			operator=(const Schedule &);

			/** Returns the set of tasks run by a core identified by its number
				@param core Core number, starting from 1
			**/
			virtual const set<const Task*>&
			getTasks(int core) const;

			/** Returns the ith task in the Schedule tasks collection
				@param id Index of the task, starting from 1, to be read from the Schedule's internal collection of Task
			**/
			virtual const Task&
			getTask(int id) const;

			/** Returns the tasks contained in a shared memory island, identified by the set of cores that forms this island.
				@param islands Set of core id (starting from 1) that form a shared memory island 
				@param pt Execution platform that corresponds to this schedule
			**/
			virtual set<const Task*>
			getTasksSharedMemoryIsland(const set<int>& islands, const Platform &pt) const;

			/** Returns the set of Task that are mapped to other shared memory islands than the island collection given as parameter, and produce data consumed by tasks mapped in the set of islands given as parameters
				@param islands Set of islands
				@param tg Taskgraph that describe links between tasks
				@param pt Platform description that defines shared memory islands
			**/
			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandProducers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const;

			/** Returns the set of Task that are mapped to another shared memory island than Task t, and produce data that Task t consumes
				@param t Task for which this function computes the set of remote tasks that produce data for it.
				@param tg Taskgraph that defines the links between tasks
				@param pt Platform that defines the core shared memory islands
			**/
			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandTaskProducers(const Task &t, const Taskgraph &tg, const Platform &pt) const;

			/** Returns the set of Task that are mapped to other shared memory islands than the island collection given as parameter, and consume data produced by tasks mapped in the set of islands given as parameters
				@param islands Set of islands
				@param tg Taskgraph that describe links between tasks
				@param pt Platform description that defines shared memory islands
			**/
			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandConsumers(const set<int> &islands, const Taskgraph &tg, const Platform &pt) const;

			/** Returns the set of Task that are mapped to another shared memory island than Task t, and consume data that Task t produces
				@param t Task for which this function computes the set of remote tasks that consume data from it
				@param tg Taskgraph that defines the links between tasks
				@param pt Platform that defines the core shared memory islands
			**/
			virtual multiset<const Task*>
			getRemoteSharedMemoryIslandTaskConsumers(const Task &t, const Taskgraph &tg, const Platform &pt) const;

			/** Returns the set of cores that execute a Task
				@param t Task that is run by the set of cores returned by the function
			**/
			virtual const set<int>
			getCores(const Task &t) const;

			/** Computes the starting time of each task in a schedule and add this starting time to a copy of the algebraic form schedule
				@param data Schedule in algebraic form that lacks the starting time of tasks
				@param tg Taskgraph that contains extended information about tasks
				@param platform Platform description that matches the algebraic schedule
			**/
			static
			Algebra
			addStartTime(const Algebra &data, const Taskgraph &tg, const Platform &platform);

		protected:
			std::string name, appName;
			table schedule;
			set<Task> tasks;
			std::map<int, set<const Task*> > core_tasks;

			/** Set the schedule of this class instance from a collection of task sequences. This method duplicates the tasks so the Schedule instance owns all instances of Tasks that the schedule refers to
			**/
			virtual void
			setSchedule(const table&);
		private:
			void buildFromAlgebra(const string &name, const string &appName, const Algebra &data);
	};
}

#endif
