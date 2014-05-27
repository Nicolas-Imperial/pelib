#include <AmplInput.hpp>
#include <Scalar.hpp>
#include <Vector.hpp>
#include <Set.hpp>
#include <Matrix.hpp>
#include <Algebra.hpp>

#include "make_algebra.cpp"

using namespace pelib;

int
main(int argc, char **argv)
{
	AmplInput input;

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	input.dump(std::cout, buildAlgebra());

	return EXIT_SUCCESS;
}
