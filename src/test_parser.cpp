#include <iostream>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <Scalar.hpp>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>

#include <GraphML.hpp>
#include <AmplArchitecture.hpp>
#include <XMLSchedule.hpp>

#include <AmplInputData.hpp>
#include <AmplInputScalar.hpp>
#include <AmplInputVector.hpp>
#include <AmplInputMatrix.hpp>

#include <AmplOutputScalar.hpp>
#include <AmplOutputVector.hpp>
#include <AmplOutputMatrix.hpp>

#include <Scalar.hpp>
#include <Set.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>

#include <unit_test.h>

using namespace pelib;
using namespace std;

static AmplInput input(AmplInput::intFloatHandlers());
static AmplOutput output(AmplOutput::intFloatHandlers());

// Include cpp format input
// Import the buildAlgebra() function
#include "make_algebra.cpp"

extern char _binary_fft_n15_1_graphml_start;
extern size_t _binary_fft_n15_1_graphml_size;
std::string string_taskgraph_graphml;
std::istringstream istream_taskgraph_graphml;

extern char _binary_fft_n15_1_input_dat_start;
extern size_t _binary_fft_n15_1_input_dat_size;
std::string string_taskgraph_amplinput;
std::istringstream istream_taskgraph_amplinput;

extern char _binary_fft_n15_1_output_dat_start;
extern size_t _binary_fft_n15_1_output_dat_size;
std::string string_taskgraph_amploutput;
std::istringstream istream_taskgraph_amploutput;

extern char _binary_p8_dat_start;
extern size_t _binary_p8_dat_size;
std::string string_architecture;
std::istringstream istream_architecture;

extern char _binary_schedule_xml_start;
extern size_t _binary_schedule_xml_size;
std::string string_schedule_xml;
std::istringstream istream_schedule_xml;

extern char _binary_crown_dat_start;
extern size_t _binary_crown_dat_size;
std::string string_schedule_crown;
std::istringstream istream_schedule_crown;

extern char _binary_schedule_dat_start;
extern size_t _binary_schedule_dat_size;
std::string string_schedule_amploutput;
std::istringstream istream_schedule_amploutput;

void
test_init()
{
	std::cout << std::setprecision(6)
 		<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);
}

void
test_setup()
{
	string_taskgraph_graphml = std::string(&_binary_fft_n15_1_graphml_start).substr(0, (size_t)(&_binary_fft_n15_1_graphml_size));
	istream_taskgraph_graphml.clear();
	istream_taskgraph_graphml.str(string_taskgraph_graphml);
	
	string_taskgraph_amplinput = std::string(&_binary_fft_n15_1_input_dat_start).substr(0, (size_t)(&_binary_fft_n15_1_input_dat_size));
	istream_taskgraph_amplinput.clear();
	istream_taskgraph_amplinput.str(string_taskgraph_amplinput);
	
	string_taskgraph_amploutput = std::string(&_binary_fft_n15_1_output_dat_start).substr(0, (size_t)(&_binary_fft_n15_1_output_dat_size));
	istream_taskgraph_amploutput.clear();
	istream_taskgraph_amploutput.str(string_taskgraph_amploutput);
	
	string_architecture = std::string(&_binary_p8_dat_start).substr(0, (size_t)(&_binary_p8_dat_size));
	istream_architecture.clear();
	istream_architecture.str(string_architecture);
	
	string_schedule_xml = std::string(&_binary_schedule_xml_start).substr(0, (size_t)(&_binary_schedule_xml_size));
	istream_schedule_xml.clear();
	istream_schedule_xml.str(string_schedule_xml);
	
	string_schedule_crown = std::string(&_binary_crown_dat_start).substr(0, (size_t)(&_binary_crown_dat_size));
	istream_schedule_crown.clear();
	istream_schedule_crown.str(string_schedule_crown);
	
	string_schedule_amploutput = std::string(&_binary_schedule_dat_start).substr(0, (size_t)(&_binary_schedule_dat_size));
	istream_schedule_amploutput.clear();
	istream_schedule_amploutput.str(string_schedule_amploutput);
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

int
parse_and_convert_graphml()
{
	Taskgraph tg_graphml = GraphML().parse(istream_taskgraph_graphml);
	Architecture arch = AmplArchitecture().parse(istream_architecture);
	
	Algebra tg_graphml_algebra = tg_graphml.buildAlgebra(arch);
	Taskgraph tg_from_algebra(tg_graphml_algebra);

	string efficiency = tg_graphml.getTasks().begin()->getEfficiencyString();
	tg_from_algebra.setMakespanCalculator("class:fft");
	set<Task> &tasks = tg_from_algebra.getTasks();
	for(set<Task>::iterator i = tasks.begin(); i != tasks.end(); i++)
	{
		Task t = *i;
		t.setEfficiencyString(efficiency);
		tasks.erase(i);
		tasks.insert(t);
	}
	
	stringstream reference;
	GraphML().dump(reference, tg_from_algebra);
	
	return reference.str().compare(string_taskgraph_graphml) == 0;

//	Algebra taskgraph_algebra = AmplInput(AmplInput::floatHandlers()).parse(istream_taskgraph_amplinput);

#if 0
	cout << string_taskgraph_graphml << endl;
	cout << "===============" << endl;
	cout << string_taskgraph_amplinput << endl;
	cout << "===============" << endl;
	cout << string_taskgraph_amploutput << endl;
	cout << "===============" << endl;
	cout << string_architecture << endl;
	cout << "===============" << endl;
	cout << string_schedule_xml << endl;
	cout << "===============" << endl;
	cout << string_schedule_crown << endl;
	cout << "===============" << endl;
	cout << string_schedule_amplinput << endl;
	cout << "===============" << endl;
#endif
}

int
parse_and_convert_schedule()
{
	Taskgraph tg_graphml = GraphML().parse(istream_taskgraph_graphml);
	Architecture arch = AmplArchitecture().parse(istream_architecture);
	Algebra ampl_schedule = AmplOutput(AmplOutput::floatHandlers()).parse(istream_schedule_amploutput);
	Schedule schedule("converted_from_ampl", tg_graphml, ampl_schedule);
	
	stringstream reference;
	XMLSchedule().dump(reference, schedule);
	
	return reference.str().compare(string_schedule_xml) == 0;
}

void
test_run()
{
	test(parse_and_convert_graphml);
	test(parse_and_convert_schedule);
}
