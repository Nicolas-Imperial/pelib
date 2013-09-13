#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <AmplData.hpp>
#include <ParamScalar.hpp>
#include <ParamVector.hpp>
#include <ParamMatrix.hpp>
#include <OutputScalar.hpp>
#include <OutputVector.hpp>

using namespace pelib;

int
main(int argc, char **argv)
{
	std::ifstream myfile;
	myfile.open (argv[1], std::ios::in);
	AmplData data;

	try {
		data.parse(myfile);
	} catch(ParseException &e)
	{
		std::cerr << e.what() << std::endl;
	}
	myfile.close();

/*
	std::map<std::string, ParamScalar<int>* > scalarRecords = data.getRecords<ParamScalar<int> >();
	ParamScalar<int>* n = scalarRecords.find("n")->second;
	std::map<std::string, ParamVector<int, int>* > vectorRecords = data.getRecords<ParamVector<int, int> >();
	ParamVector<int, int>* Wi = vectorRecords.find("Wi")->second;
*/

	std::cout << data << std::endl;

	std::cout << data.find<ParamScalar<int> >("b")->getValue() << std::endl;
	std::cout << data.find<ParamVector<int, int> >("Wi")->getSize() << std::endl;
	std::cout << data.find<ParamMatrix<int, int, float> >("e")->getRowSize() << std::endl;

	OutputScalar<int> outputScalar(data.find<ParamScalar<int> >("n"));
	outputScalar.setName("nn");
	std::cout << outputScalar << std::endl;

	OutputVector<int, int> outputVector(data.find<ParamVector<int, int> >("Wi"));
	outputVector.setName("yy");
	std::cout << outputVector << std::endl;
	
	return EXIT_SUCCESS;
}

