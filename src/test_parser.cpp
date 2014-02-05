#include <iostream>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>

#include <AmplInputData.hpp>
#include <AmplInputScalar.hpp>
#include <AmplInputVector.hpp>
#include <AmplInputMatrix.hpp>

#include <AmplOutputScalar.hpp>
#include <AmplOutputVector.hpp>
#include <AmplOutputMatrix.hpp>

#include <mapping.hpp>
#include <frequency.hpp>

#include <unit_test.h>

using namespace pelib;
using namespace std;

static AmplInput input;
static AmplOutput output;
static Record record;

void
test_init()
{
	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)
 	<< std::setiosflags(std::ios::fixed)
	<< std::setiosflags(std::ios::showpoint);

	// Open input file
	std::ifstream myfile;
	myfile.open ("unit_test.dat", std::ios::in);

	// Load input file
	try {
		record = input.parse(myfile);
	} catch(ParseException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	// Close input file
	myfile.close();
}

void
test_setup()
{
  // Allocate and initialize your test stack before each test
}

void
test_teardown()
{
  // Do not forget to free your stacks after each test
  // to avoid memory leaks as now
}

void
test_finalize()
{
  // Destroy properly your test batch
}

static float
quality(Record& schedule)
{
	int n = schedule.find<Scalar<int> >("n")->getValue();
	int p = schedule.find<Scalar<int> >("p")->getValue();
	float M = schedule.find<Scalar<float> >("M")->getValue();
	Vector<int, int> tau = schedule.find<Vector<int, int> >("Tau");
	Vector<int, int> Wi = schedule.find<Vector<int, int> >("Wi");
	Vector<int, int> wi = schedule.find<Vector<int, int> >("wi");
	Vector<int, int> frequency = schedule.find<Vector<int, int> >("frequency");
	Vector<int, int> group = schedule.find<Vector<int, int> >("group");
	Matrix<int, int, float> e = schedule.find<Matrix<int, int, float> >("e");
    
	double static_energy = p * M;
	double dynamic_energy = 0;
	float alpha = 3.0, zeta = 0.1;

	// Now evaluate the quality of this schedule
	for(map<int, int>::const_iterator i = group.getValues().begin(); i != group.getValues().end(); i++)
	{   
		int task = i->first;

		int tau_t = tau.find(task);
		int width_t = wi.find(task);
		double e_wt = e.find(width_t, task);
		int f_t = frequency.find(task);

		double area = (double)tau_t / (double)(e.find(width_t, task)) / (double)frequency.find(task);
		static_energy -= area;
		dynamic_energy += area * pow(f_t, alpha);

		//cerr << "#Task " << task << ", tau " << tau_t << ", width " << width_t << ", e " << e_wt << ", area " << area << ", f " << f_t << ", energy " << area * pow(frequency.find(task), alpha) << endl;
		//cerr << static_energy << endl;
		//cerr << dynamic_energy << endl;
	}   

	return dynamic_energy + zeta * static_energy;
}

int
record_copy()
{
	Record copy;
	// We use an AMPL output, output format
	RecordOutput &out = input;

	std::map<std::string, const Data * const> records = record.getAllRecords();
	for (std::map<std::string, const Data * const>::const_iterator rec = records.begin(); rec != records.end(); rec++)
	{
		cout << rec->first << endl;
		std::cout << (const Data * const)rec->second << std::endl;
		out.dump(cout, rec->second);
	}
	cout << endl;	

	//std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl;
	for (std::map<std::string, const Data * const>::const_iterator rec = record.getAllRecords().begin(); rec != record.getAllRecords().end(); rec++)
	{
		std::cout << rec->first << std::endl;
		std::cout << (const Data * const)rec->second << std::endl;
		out.dump(cout, rec->second);
	}
	cout << endl;

	out.dump(cout, record);

	return 0;
}

void
test_run()
{
	test(record_copy);
}
