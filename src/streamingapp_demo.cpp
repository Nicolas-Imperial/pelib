#include <StreamingApp.hpp>
#include <StreamingAppHandler.hpp>
#include <AmplArchitecture.hpp>
#include <Architecture.hpp>

using namespace pelib;
using namespace std;

int
main(int argc, char **argv)
{
	StreamingAppHandler input;

	std::ifstream arch_file;
	arch_file.open (argv[1], std::ios::in);
	input.parse(arch_file);

	AmplArchitecture ampl_arch;
	Architecture *arch;
	arch = ampl_arch.parse(arch_file);
	arch_file.close();

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	ampl_arch.dump(cout, arch);
	delete arch;

	return EXIT_SUCCESS;
}

