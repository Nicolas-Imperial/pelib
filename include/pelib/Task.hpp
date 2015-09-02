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
	class Link;

	class Task
	{
		public:
			Task(const std::string id);
			virtual ~Task();
			
			virtual double
			getFrequency() const;

			virtual void
			setFrequency(double frequency);

			virtual double
			getWidth() const;

			virtual void
			setWidth(double width);

			virtual std::string
			getModule() const;

			virtual void
			setModule(const std::string name);

			virtual std::string
			getName() const;

			virtual std::string
			getEfficiencyString() const;

			virtual void
			setEfficiencyString(const std::string efficiencyString);

			virtual double
			getEfficiency(int p, double def = very_small) const;
			
			virtual double
			getWorkload() const;

			virtual void
			setWorkload(double workload);
			
			virtual double
			getMaxWidth() const;

			virtual void
			setMaxWidth(double maxWidth);

			virtual double
			getStartTime() const;

			virtual void
			setStartTime(double startTime);

			virtual double
			runtime(double width = 1, double frequency = 1) const;

			virtual bool
			operator<(const Task &other) const;

			virtual const std::set<const Link*>&
			getProducers() const;

			virtual const std::set<const Link*>&
			getConsumers() const;

			virtual std::set<const Link*>&
			getProducers();

			virtual std::set<const Link*>&
			getConsumers();

	    		virtual bool
			operator==(const Task &other) const;
			
		protected:
			double frequency, width;
			double workload, maxWidth;
			float start_time;
			std::string name, module, efficiencyString;
			std::set<const Link*> consumers, producers;
			
			static const float very_small = 1e-6;
			
		private:
	};
}

#endif
