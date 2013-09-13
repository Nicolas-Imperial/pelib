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

	std::cout << data.find<ParamMatrix<int, int, float> >("e")->getColSize() << std::endl;

	OutputVector<int, int> vector(data.find<ParamVector<int, int> >("Wi"));
	vector.setName("yy");
	std::cout << vector << std::endl;
	
	return EXIT_SUCCESS;
}

