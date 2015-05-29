#include <iostream>
#include <sstream>

#include <exprtk.hpp>

#include <Task.hpp>
#include <ParseException.hpp>

using namespace std;

struct print : public exprtk::ifunction<double>
{
	public:
		print(ostream &stream)
			: exprtk::ifunction<double>(1), stream(stream)
		{}  

		inline double operator()(const double &value) 
		{   
			stream << value << endl;
			return value;
		}   

	private:
		ostream &stream;
};

namespace pelib
{
	Task::Task(const int id, const std::string taskId)
	{
		if(id <= 0)
		{
			throw ParseException("A task ID must be stricly higher than 0");
		}
		
		this->id = id;
		this->taskId = taskId;
		this->name = taskId;
		this->frequency = 1;
		this->width = 1;
		this->maxWidth = 1;
		this->workload = 1;
		this->efficiencyString = "fml:p <= 1 ? 1 : 1e-6";
		this->start_time = 0;
	}

	Task::Task(const int id)
	{
		if(id <= 0)
		{
			throw ParseException("A task ID must be stricly higher than 0");
		}
		
		this->id = id;
		this->frequency = 1;
		this->width = 1;
		this->maxWidth = 1;
		this->workload = 1;
		this->efficiencyString = "fml:p <= 1 ? 1 : 1e-6";

		stringstream ss;
		ss << id;
		int id_size = ss.str().size();
		ss.str(std::string());
		ss.clear();
		ss << "task";
		
		for(int i = 0; i < id_size; i++)
		{
			ss << "_";
		}
		ss << id;
		string base = ss.str(); 

		this->name = base + "_name";
		this->taskId = base + "_id";
		this->start_time = 0;
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
		if(getEfficiencyString().substr(0, 4) == "fml:")
		{
			string formula = getEfficiencyString().substr(4);

			print print_cout(cout);
			print print_cerr(cerr);

			typedef exprtk::expression<double> expression_t;
			typedef exprtk::parser<double>         parser_t;

			double W = this->getMaxWidth();
			double tau = this->getWorkload();
			expression_t expression;
			double pp = p;

			exprtk::symbol_table<double> symbol_table;
			symbol_table.add_variable("W", W);
			symbol_table.add_variable("p", pp);
			symbol_table.add_variable("tau", tau);
			symbol_table.add_function("cout", print_cout);
			symbol_table.add_function("cerr", print_cerr);

			expression.register_symbol_table(symbol_table);

			parser_t parser;

			parser.compile(formula, expression);	
			double output =  expression.value();

			if(std::isnan(output)) // TODO: catch whatever mathparser would throw
			{
				throw new ParseException("Could not parse efficiency formula \"" + getEfficiencyString() + "\"");
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
