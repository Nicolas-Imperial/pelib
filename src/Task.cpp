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


#include <iostream>
#include <sstream>
#include <string>

#include <pelib/pelib_exprtk.hpp>

#include <pelib/Task.hpp>
#include <pelib/ParseException.hpp>

#ifdef debug
#undef debug
#endif

#define debug(expr) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #expr << " = \"" << (expr) << "\"." << endl;

using namespace std;

namespace pelib
{
	const float Task::very_small = 1e-6;

	Task::Task(const std::string &name)
	{
		this->name = name;
		this->module = "dummy";
		this->frequency = 1;
		this->width = 1;
		this->maxWidth = 1;
		this->workload = 1;
		this->start_workload = 0;
		this->efficiencyString = "exprtk:p <= 1 ? 1 : 1e-6";
		this->start_time = 0;
	}

	Task::Task(const Task &task)
	{
		this->name = task.getName();
		this->module = task.getModule();
		this->frequency = task.getFrequency();
		this->width = task.getWidth();
		this->maxWidth = task.getMaxWidth();
		this->workload = task.getWorkload();
		this->start_workload = task.getStartWorkload();
		this->efficiencyString = task.getEfficiencyString();
		this->start_time = task.getStartTime();

		this->consumers = task.getConsumers();
		this->producers = task.getProducers();
	}

	Task::~Task()
	{
		/* Do nothing */
	}
	
	double
	Task::getFrequency() const
	{
		return frequency;
	}

	void
	Task::setFrequency(double frequency)
	{
		this->frequency = frequency; 
	}

	double
	Task::getWidth() const
	{
		return width;
	}

	void
	Task::setWidth(double width)
	{
		this->width = width; 
	}

	std::string
	Task::getName() const
	{
		return this->name;
	}

	/*
	int
	Task::getName() const
	{
		return this->id;
	}*/
	
	std::string
	Task::getEfficiencyString() const
	{
		return this->efficiencyString;
	}

	void
	Task::setEfficiencyString(const std::string &efficiencyString)
	{
		//std::string old = getEfficiencyString();
		
		this->efficiencyString = string(efficiencyString);
		
		// Attemps to compute an efficiency
		/*
		try
		{
			getEfficiency(1);
		} catch (ParseException &e)
		{
			// Restore the old efficiency and bounce the exception
			this->efficiencyString = old;
			throw e;
		}
		*/
	}

	double
	Task::getEfficiency(int p, double def) const
	{
		if(getEfficiencyString().find("exprtk") == 0)
		{
			string formula = getEfficiencyString().substr(string("exprtk").size() + 1);
			double W = this->getMaxWidth();
			double tau = this->getWorkload();

			double output = parseEfficiency(formula, W, tau, p);

			if(std::isnan(output))
			{
				throw new ParseException("Could not parse efficiency formula \"" + getEfficiencyString() + "\"");
			}

			if(output < very_small)
			{
				output = very_small;
			}

			return output;		
		}
		else
		{
			stringstream stream(getEfficiencyString());

			double num;
			size_t count = 1;

			while(stream >> num && count < (size_t)p)
			{
				count++;
			}

			if(count > (size_t)p)
			{
				num = def;
			}

			return num;
		}
	}

	double
	Task::getWorkload() const
	{
		return this->workload;
	}

	void
	Task::setWorkload(double workload)
	{
		this->workload = workload;
	}

	double
	Task::getStartWorkload() const
	{
		return this->start_workload;
	}

	void
	Task::setStartWorkload(double workload)
	{
		this->start_workload = workload;
	}

	double
	Task::getMaxWidth() const
	{
		return this->maxWidth;
	}

	void
	Task::setMaxWidth(double maxWidth)
	{
		this->maxWidth = maxWidth;
	}

	// Write in *number the address of the first character of the number found, or the end of the string str if no number was found.
	// Returns the number of characters to parse as number
	static size_t
	scan_for_number(const char *str, const char **number)
	{
		// Make sure the number part is initialized correctly, just in case we start the string with a number.
		*number = str;

		// Iterate until finding 0-9, + or -
		while((*str < '0' || *str > '9') && !((*str == '-' || *str == '+') && ((*(str + 1) >= '0' && *(str + 1) <= '9') || *(str + 1) == 'e' || *(str + 1) == 'E' || *(str + 1) == '.')) && *str != '\0')
		{
			str++;
			*number = str;
		}

		// Iterate until finding a dot or a e
		while(*str != '.' && *str != 'e' && *str != 'E' && *str != '\0' && *str >= '0' && *str <= '9')
		{
			str++;
		}

		// If we find a point, then none is allowed before we find a power of ten sign (e or E, i.e. 56.333E4)
		if(*str == '.')
		{
			while((*str >= '0' && *str <= '9') && *str != 'e' && *str != 'E' && *str != '\0')
			{
				str++;
			}

			// Here is the exponent part
			if(*str == 'e' || *str == 'E')
			{
				// The exponent part can begin with a + or a -
				if(*str == '+' || *str == '-')
				{
					str++;
				}

				// Then can follow either number or a point
				while((*str >= '0' && *str <= '9') && *str != '.' && *str != '\0')
				{
					str++;
				}
				
				// The exponent is decimal
				if(*str == '.')
				{
					// Read more number. No more point or exponent sign is allowed
					while((*str >= '0' && *str <= '9') && *str != '\0')
					{
						str++;
					}

					// Returns a decimal number with a decimal exponent
					return ((size_t)str - (size_t)*number) / sizeof(char);
				}

				// Returns a decimal number with an integer exponent
				return ((size_t)str - (size_t)*number) / sizeof(char);
			}

			// Returns a decimal number with no exponent
			return ((size_t)str - (size_t)*number) / sizeof(char);
		}

		// This may be an exponent
		if(*str == 'e' || *str == 'E')
		{
			// Go to next character
			str++;

			// The exponent part can begin with a + or a -
			if(*str == '+' || *str == '-')
			{
				str++;
			}

			// Then can follow either number or a point
			while((*str >= '0' && *str <= '9') && *str != '.' && *str != '\0')
			{
				str++;
			}
			
			// The exponent is decimal
			if(*str == '.')
			{
				// Read more number. No more point or exponent sign is allowed
				while((*str >= '0' && *str <= '9') && *str != '\0')
				{
					str++;
				}

				// Returns an integer number with a decimal exponent
				return ((size_t)str - (size_t)*number) / sizeof(char);
			}

			// Returns an integer number with an integer exponent
			return ((size_t)str - (size_t)*number) / sizeof(char);
		}

		// Returns a integer number with no exponent, or no number at all of *str == '\0'
		return ((size_t)str - (size_t)*number) / sizeof(char);
	}

	bool
	Task::operator<(const Task &other) const
	{
		string Me(this->getName());
		string Ot(other.getName());
		const char* me = Me.c_str(); //string(this->getName()).c_str();
		const char* ot = Ot.c_str(); //string(other.getName()).c_str();
		
		//if(string(me).compare(string(ot)) == 0)
		if(this->getName().compare(other.getName()) == 0)
		{
			return false;
		}

		while(*me != '\0' && *ot != '\0')
		{
			// Find where a number begins for both strings
			// and keep the hypethetic non-number string prefix
			const char *me_num;
			size_t me_num_length = scan_for_number(me, &me_num);
			const char *ot_num;
			size_t ot_num_length = scan_for_number(ot, &ot_num);

			// Compare the non-number string prefix for both strings
			size_t me_non_number_length = ((ptrdiff_t)me_num - (ptrdiff_t)me) / sizeof(char);
			size_t ot_non_number_length = ((ptrdiff_t)ot_num - (ptrdiff_t)ot) / sizeof(char);
			
			int diff = string(me).substr(0, me_non_number_length).compare(string(ot).substr(0, ot_non_number_length));
			if(diff != 0)
			{
				return diff < 0;
			}
			else
			{
				// We can compare the numbers
				double me_double, ot_double;
				std::istringstream(string(me_num).substr(0, me_num_length)) >> me_double;
				std::istringstream(string(ot_num).substr(0, ot_num_length)) >> ot_double;
				
				// Go check if there is more text to parse
				me = me_num + me_num_length;
				ot = ot_num + ot_num_length;

				if(me_double != ot_double)
				{
					return me_double < ot_double;
				}
			}
		}

		// If one of the string is found empty, just perform a classic string comparison
		return string(me).compare(string(ot)) < 0;
	}
    
	bool
	Task::operator==(const Task &other) const
	{
		return getName().compare(other.getName()) == 0;
	}

	std::string
	Task::getModule() const
	{
		return this->module;
	}

	void
	Task::setModule(const std::string &module)
	{
		this->module = module;
	}

	double
	Task::getStartTime() const
	{
		return this->start_time;
	}

	void
	Task::setStartTime(double startTime)
	{
		this->start_time = startTime;
	}

	double
	Task::runtime(double width, double frequency) const
	{
		double work = getWorkload();
		work = work / (width * getEfficiency((int)width));
		work = work / frequency;
		
		return work; 
	}
	
	double
	Task::startRuntime(double width, double frequency) const
	{
		double work = getStartWorkload();
		work = work / (width * getEfficiency((int)width));
		work = work / frequency;
		
		return work; 
	}
	
	const set<const Link*>&
	Task::getProducers() const
	{
		return this->producers;
	}

	const set<const Link*>&
	Task::getConsumers() const
	{
		return this->consumers;
	}

	set<const Link*>&
	Task::getProducers()
	{
		return this->producers;
	}

	set<const Link*>&
	Task::getConsumers()
	{
		return this->consumers;
	}
}
