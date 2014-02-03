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

#include <unit_test.h>

using namespace pelib;

static AmplInput input;
static AmplOutput output;
static Record rec;

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
		rec = input.parse(myfile);
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

int
record_copy()
{
	Record copy;
	// We use an AMPL output, output format
	RecordOutput &out = output;
	
	std::stringstream rec_buffer, copy_buffer;
	out.dump(rec_buffer, rec);	

	copy = rec;
	out.dump(copy_buffer, copy);

	return !rec_buffer.str().compare(copy_buffer.str());
}

void
test_run()
{
	test(record_copy);
}
