#include <AmplInput.hpp>
#include <Cpp.hpp>

using namespace pelib;

int
main(int argc, char **argv)
{
	AmplInput input(AmplInput::floatHandlers());
	Cpp output("buildAlgebra");

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	output.dump(std::cout, input.parse(std::cin));

	return EXIT_SUCCESS;
}
