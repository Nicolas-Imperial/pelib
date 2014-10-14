#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include <AmplOutput.hpp>
#include <Platform.hpp>
#include <AmplPlatform.hpp>
#include <GraphML.hpp>

int main(int argc, char **argv)
{
	using namespace std;
	using namespace pelib;

	Platform *platform, default_arch;
	AmplPlatform ai;
	
	if(argc >= 2)
	{
		ifstream tmp(argv[1]);
		platform = ai.parse(tmp);
		tmp.close();
	}
	else
	{
		cerr << "Warning: No platform information supplied. Using default values\n";
		platform = &default_arch;
	}
	
	GraphML input;
	AmplInput output(AmplInput::intFloatHandlers());

	// Set floating point var output format to fixed at 7 digits
	std::cout << std::setprecision(6)                                                                                                        
		<< std::setiosflags(std::ios::fixed)                                                                                                     
		<< std::setiosflags(std::ios::showpoint);  

	/*
	std::ifstream myfile;
	myfile.open (argv[2], std::ios::in);
	Taskgraph *tg = input.parse(myfile);
	myfile.close();
	*/
	Taskgraph *tg = input.parse(cin);

	output.dump(cout, tg->buildAlgebra(*platform));

	return 0;
}
