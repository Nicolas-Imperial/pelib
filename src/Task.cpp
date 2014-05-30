#include <muParser.h>

#include <Task.hpp>
#include <ParseException.hpp>

using namespace std;

namespace pelib
{
	Task::Task(int id, std::string taskId)
	{
		this->id = id;
		this->taskId = taskId;
		this->name = "";
		this->frequency = 1;
		this->width = 1;
		this->maxWidth = 1;
		this->workload = 1;
		this->efficiencyString = "fml:p <= ? 1 : 1e-6";
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
	Task::getTaskId() const
	{
		return this->taskId;
	}

	int
	Task::getId() const
	{
		return this->id;
	}
	
	std::string
	Task::getEfficiencyString() const
	{
		return this->efficiencyString;
	}

	void
	Task::setEfficiencyString(const std::string efficiencyString)
	{
		std::string old = getEfficiencyString();
		
		this->efficiencyString = efficiencyString;
		
		// Attemps to compute an efficiency
		try
		{
			getEfficiency(1);
		} catch (ParseException &e)
		{
			// Restore the old efficiency and bounce the exception
			this->efficiencyString = old;
			throw e;
		}
	}

	double
	Task::getEfficiency(int p, double def) const
	{
		// TODO: implement
		if(getEfficiencyString().substr(0, 4) == "fml:")
		{
			using namespace mu;
			string fml = getEfficiencyString().substr(4);
			double d_p = p;

			Parser mathparser;
			mathparser.SetExpr(fml);
			// This is where you may define additional parameters
			// for the efficiency function.
			// It is allowed to define parameters not present in the
			// formula, it will just be ignored.
			mathparser.DefineVar("p", &d_p);

			try
			{
				return mathparser.Eval();
			} catch (ParseException &e) // TODO: catch whatever mathparser would throw
			{
				throw new ParseException("Could not parse efficiency formula \"" + getEfficiencyString() + "\"");
			}
		}
		else
		{
			stringstream stream(getEfficiencyString());

			double num;
			size_t count = 0;

			while(stream >> num && count < (size_t)p)
			{
				count++;
			}

			if(count >= (size_t)p)
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
	Task::getMaxWidth() const
	{
		return this->maxWidth;
	}

	void
	Task::setMaxWidth(double maxWidth)
	{
		this->maxWidth = maxWidth;
	}

	bool
	Task::operator<(const Task &other) const
	{
		return getId() < other.getId();
	}
    
    bool
	Task::operator==(const Task &other) const
	{
		return getId() == other.getId();
	}

	std::string
	Task::getName() const
	{
		return this->name;
	}

	void
	Task::setName(const std::string name)
	{
		this->name = name;
	}

	bool
	Task::runtime(double width, double frequency) const
	{
		double work = getWorkload();
		work = work / (width * getEfficiency(width));
		work = work / frequency;
		
		return work; 
	}
}
