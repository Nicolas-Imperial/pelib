#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include <Architecture.hpp>
#include <AmplArchitecture.hpp>
#include <GraphML.hpp>

int main(int argc, char **argv)
{
	using namespace std;
	using namespace pelib;

	Architecture *architecture;
	AmplArchitecture ai;
	if(argc == 2)
	{
		ifstream tmp(argv[1]);
		architecture = ai.parse(tmp);
		tmp.close();
	}
	else
	{
		cerr << "Warning: No architecture information supplied. Using default values\n";
	}
	
	GraphML input;
	AmplInput output;

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
		<< std::setiosflags(std::ios::fixed)                                                                                                     
		<< std::setiosflags(std::ios::showpoint);  

	Taskgraph *tg = input.parse(cin);

	output.dump(cout, tg->buildAlgebra(*architecture));

	return 0;
}
