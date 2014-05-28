#include <sstream>

#include <StreamingApp.hpp>
#include <StreamNet.hpp>
#include <AmplArchitecture.hpp>
#include <Architecture.hpp>

#include <AmplInput.hpp>
#include <AmplInputScalar.hpp>

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

	std::ifstream arch_file;
	arch_file.open (argv[1], std::ios::in);
	app = input.parse(arch_file, input.getEmptyStream(), input.getEmptyStream());
	arch_file.close();

	input.dump(cout, app.getArchitecture());
	
	return EXIT_SUCCESS;
}

