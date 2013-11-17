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

using namespace pelib;

Record
parse(RecordParser &parser, std::istream &input)
{
	Record rec;
	
	try {
		rec = parser.parse(input);
	} catch(ParseException &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return rec;
}

int
main(int argc, char **argv)
{
	AmplInput input;
	AmplOutput output;
	Record rec;

	// We use an AMPL output, output format
	RecordOutput &out = input;

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
 	<< std::setiosflags(std::ios::fixed)                                                                                                     
	<< std::setiosflags(std::ios::showpoint);

	// Open input file
	std::ifstream myfile;
	myfile.open (argv[1], std::ios::in);

	// Load input file
	rec = parse(input, myfile);

	// Close input file
	myfile.close();

	out.dump(std::cout, rec);

#if 0
	// Output one raw value in parameters b, Wi and e
	std::cout << rec.find<Scalar<int> >("b")->getValue() << std::endl;
	std::cout << rec.find<Vector<int, int> >("Wi")->getSize() << std::endl;
	std::cout << rec.find<Matrix<int, int, float> >("e")->getRowSize() << std::endl;

	// Extract, rename and output a few parameters
	Scalar<int> nn(rec.find<Scalar<int> >("n"));
	nn.setName("nn");
	out.dump(std::cout, &nn);

	Vector<int, int> yy(rec.find<Vector<int, int> >("Wi"));
	yy.setName("yy");
	out.dump(std::cout, &yy);
	
	Matrix<int, int, float> ee(rec.find<Matrix<int, int, float> >("e"));
	ee.setName("ee");
	out.dump(std::cout, &ee);
#endif

	return EXIT_SUCCESS;
}

