#include <pelib/AmplInput.hpp>
#include <pelib/Scalar.hpp>
#include <pelib/Vector.hpp>
#include <pelib/Set.hpp>
#include <pelib/Matrix.hpp>
#include <pelib/Algebra.hpp>

#include "make_algebra.cpp"

using namespace pelib;

int
main(int argc, char **argv)
{
	AmplInput input(AmplInput::intFloatHandlers());

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	input.dump(std::cout, buildAlgebra());

	return EXIT_SUCCESS;
}
