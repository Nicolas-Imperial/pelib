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
#include <set>

#ifndef PELIB_TASK
#define PELIB_TASK

namespace pelib
{
	// Forward declaration
	/** Forward declaration of a link between tasks **/
	class Link;

	/** Models a moldable task of a streaming application **/
	class Task
	{
		public:
			/** Constructor
				@param id Identifier of the task
			**/
			Task(const std::string &id);
			/** Copy constructor **/
			Task(const Task&);

			/** Destructor **/
			virtual ~Task();
			
			/** Return the frequency, in KHz, the task runs at **/
			virtual double
			getFrequency() const;

			/** Sets the frequency at which the task runs **/
			virtual void
			setFrequency(double frequency);

			/** Get the number of processors that run this task **/
			virtual double
			getWidth() const;

			/** Set the number of processors that run this task **/
			virtual void
			setWidth(double width);

			/** Gets the name of the module containing the code run by this task **/
			virtual std::string
			getModule() const;

			/** Sets the name of the module containing the code run by this task **/
			virtual void
			setModule(const std::string &name);

			/** Gets the name (identifier string) of the task **/
			virtual std::string
			getName() const;

			/** Gets the formula that defines the overhead of the parallelization of this task **/
			virtual std::string
			getEfficiencyString() const;

			/** Sets the formula that defines the overhead of the parallelization of this task **/
			virtual void
			setEfficiencyString(const std::string &efficiencyString);

			/** Computes the efficiency of the task, given its efficiency formula
				@param p Get efficiency of the task for p cores
				@param def Default efficiency if p is higher than the maximal width of the task
			**/
			virtual double
			getEfficiency(int p, double def = very_small) const;
			
			/** Gets the workload, in number of instructions, of the task **/
			virtual double
			getWorkload() const;

			/** Sets the workload, in number of instructions, of the task **/
			virtual void
			setWorkload(double workload);
			
			/** Gets the maximum number of cores that can run this task in parallel in an efficient manner **/
			virtual double
			getMaxWidth() const;

			/** Sets the maximum number of cores that can run this task in parallel in an efficient manner **/
			virtual void
			setMaxWidth(double maxWidth);

			/** Gets the time at which this task begins to work, relative to the beginning of a pipeline round **/
			virtual double
			getStartTime() const;

			/** Sets the time at which this task begins to work, relative to the beginning of a pipeline round **/
			virtual void
			setStartTime(double startTime);

			/** Computes and returns the global run time of this task assuming a given width and frequency
				@param width Assume the task run on that many cores
				@param frequency Assumes the task runs at this frequency
			**/
			virtual double
			runtime(double width = 1, double frequency = 1) const;

			/** Allows the comparison of tasks. Used with ==, allows the test of difference **/
			virtual bool
			operator<(const Task &other) const;

			/** Get the set of task that produce inputs for this task **/
			virtual const std::set<const Link*>&
			getProducers() const;

			/** Get the set of task that consume outputs from this task **/
			virtual const std::set<const Link*>&
			getConsumers() const;

			/** Get the set of task that produce inputs for this task **/
			virtual std::set<const Link*>&
			getProducers();

			/** Get the set of task that consume outputs from this task **/
			virtual std::set<const Link*>&
			getConsumers();

			/** Allows the comparison of tasks. Used with <, allows the test of difference **/
	    		virtual bool
			operator==(const Task &other) const;
			
		protected:
			/** Frequency and width allocated to this task **/
			double frequency, width;
			/** Workload in number of instructions and maximum number of cores able to run this task **/
			double workload, maxWidth;
			/** Time at which this task starts **/
			float start_time;
			/** Name, source code module name and efficiency formula of the task **/
			std::string name, module, efficiencyString;
			/** Sets of producers and consumers linked to this task **/
			std::set<const Link*> consumers, producers;

			/** Default efficiency value when computing efficiency for more cores that this task can support **/			
			static const float very_small;
			
		private:
	};
}

#endif
