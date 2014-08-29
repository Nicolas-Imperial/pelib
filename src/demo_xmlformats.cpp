#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include "Taskgraph.hpp"
#include "XMLSchedule.hpp"

using namespace pelib;

int
main(int argc, char **argv)
{
	XMLSchedule schedule;
	Taskgraph tg;
	schedule.parse(std::cin, tg);

	return EXIT_SUCCESS;
}

