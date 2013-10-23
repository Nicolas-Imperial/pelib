#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>

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

	std::ifstream myfile;
	myfile.open (argv[1], std::ios::in);

	rec = parse(input, myfile);
	output.dump(std::cout, rec);
	//input.dump(std::cout, rec);
	
	myfile.close();

#if 0
	AmplInput().dump(std::cout, rec);

	std::cout << rec.find<Scalar<int> >("b")->getValue() << std::endl;
	std::cout << rec.find<Vector<int, int> >("Wi")->getSize() << std::endl;
	std::cout << rec.find<Matrix<int, int, float> >("e")->getRowSize() << std::endl;

	Scalar<int> nn(rec.find<Scalar<int> >("n"));
	nn.setName("nn");
	AmplOutputScalar<int>().dump(std::cout, &nn);

	Vector<int, int> yy(rec.find<Vector<int, int> >("Wi"));
	yy.setName("yy");
	AmplOutputVector<int, int>().dump(std::cout, &yy);
	
	Matrix<int, int, float> ee(rec.find<Matrix<int, int, float> >("e"));
	ee.setName("ee");
	AmplOutputMatrix<int, int, float>().dump(std::cout, &ee);
#endif
	return EXIT_SUCCESS;
}

