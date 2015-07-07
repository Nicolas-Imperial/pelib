#include "unit_test.h"
#include <stdio.h>

static int test_final; 

int
test_record(int res)
{
	test_final = test_final && res;
	return res;
}

int main(int argc, char **argv)
{
	// Initialize the test batch
	test_init();

	// Initialize final report to 1 and run all test suit
	test_final = 1;
	test_run();

	// Destroy the test environment
	test_finalize();

	// Return 0 if final is still one, else report an error
	return !test_final;
}

