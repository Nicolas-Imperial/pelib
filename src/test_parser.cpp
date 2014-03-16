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
using namespace std;

static AmplInput input;
static AmplOutput output;
static Record record;

void
test_init()
{
	Scalar<int> p("p", 6);
	record.insert(&p);
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
record_copy_constructor()
{
	Record local = record;
	return 1;
}

void
test_run()
{
	test(record_copy_constructor);
}
