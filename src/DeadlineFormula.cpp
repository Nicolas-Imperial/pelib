#include <cmath>

#include <pelib/DeadlineFormula.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Set.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Vector.hpp>
#include <pelib/ParseException.hpp>
#include <pelib/CastException.hpp>

#include <pelib/exprtk.hpp>

#if 0
#define debug(var) cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " << #var << " = \"" << var << "\"" << endl;
#else
#define debug(var)
#endif

using namespace pelib;
using namespace std;

struct e: public exprtk::ifunction<double>
{
	public:
		e(const set<Task> &tasks, const Platform &arch)
			: exprtk::ifunction<double>(2), tasks(tasks), arch(arch)
		{}  

		inline double operator()(const string &id, const double &p) 
		{   
			Task key(id);
			return tasks.find(key)->getEfficiency((int)p);
		}   

	private:
		const set<Task> &tasks;
		const Platform &arch;
};

struct print: public exprtk::ifunction<double>
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

DeadlineFormula::DeadlineFormula(string formula) : formula(formula) {}

double
DeadlineFormula::calculate(const Taskgraph &tg, const Platform &arch) const
{
	e matrix_e(tg.getTasks(), arch);
	print print_cout(cout);
	print print_cerr(cerr);

	typedef exprtk::expression<double> expression_t;
	typedef exprtk::parser<double>         parser_t;

	if(!arch.isHomogeneous())
	{
		throw CastException("Cannot compute a deadline for a taskgraph with a heterogeneous platform.");
	}

	// Vector of tasks ids, vector of task workloads tau, vector of task max load
	vector<double> n, p, F, tau, W;
	for(set<Task>::const_iterator iter = tg.getTasks().begin(); iter != tg.getTasks().end(); iter++)
	{
		n.push_back((double)std::distance(tg.getTasks().begin(), iter) + 1);
		tau.push_back((double)iter->getWorkload());
		W.push_back((double)iter->getMaxWidth());
	}

	for(double i = 1; i <= (double)arch.getCores().size(); i++)
	{
		p.push_back(i);
	}

	for(set<float>::const_iterator i = (*(arch.getCores().begin()))->getFrequencies().begin(); i != (*(arch.getCores().begin()))->getFrequencies().end(); i++)
	{
		F.push_back(*i);
	}

	expression_t expression;

	exprtk::symbol_table<double> symbol_table;
	symbol_table.add_vector("n", n);
	symbol_table.add_vector("p", p);
	symbol_table.add_vector("F", F);
	symbol_table.add_vector("tau", tau);
	symbol_table.add_vector("W", W);
	symbol_table.add_function("e", matrix_e);
	symbol_table.add_function("cout", print_cout);
	symbol_table.add_function("cerr", print_cerr);

	expression.register_symbol_table(symbol_table);

	parser_t parser;

	parser.compile(formula, expression);	
	double output =  expression.value();

	if(std::isnan(output))
	{
		throw ParseException("Deadline type not recognized: " + formula);
	}	

	return output;
}
