#include <sstream>

#include <StreamingApp.hpp>
#include <StreamNet.hpp>
#include <AmplArchitecture.hpp>
#include <Architecture.hpp>

#include <AmplOutput.hpp>

using namespace pelib;
using namespace std;

int
main(int argc, char **argv)
{
	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)
	 	<< std::setiosflags(std::ios::fixed)
		<< std::setiosflags(std::ios::showpoint);
	
	StreamNet input;
	StreamingApp app;

	std::ifstream tg_file;
	tg_file.open (argv[1], std::ios::in);
	std::ifstream arch_file;
	arch_file.open (argv[2], std::ios::in);
	std::ifstream sched_file;
	sched_file.open (argv[3], std::ios::in);
	
	//app = input.parse(tg_file, arch_file, sched_file);
	app = input.parse(input.getEmptyStream(), arch_file, sched_file);
	
	tg_file.close();
	arch_file.close();
	sched_file.close();

	input.dump(cout, app.getArchitecture());
	input.dump(cout, app.getSchedule());

	AmplOutput().dump(cout, app.getArchitecture().buildAlgebra());
	AmplOutput().dump(cout, app.getSchedule().buildAlgebra());
	
	return EXIT_SUCCESS;
}

