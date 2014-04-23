#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>


#include <AmplOutput.hpp>
#include "Taskgraph.hpp"

int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;
  size_t processors = 128;
  if(argc == 2)
    {
      processors = atoi(argv[1]);
    }
  Taskgraph input;
  AmplOutput output;
  
  
  // Set floating point var output format to fixed at 7 digits
  std::cout << std::setprecision(6)                                                                                                        
	    << std::setiosflags(std::ios::fixed)                                                                                                     
	    << std::setiosflags(std::ios::showpoint);  
  
  output.dump(cout,input.parse(cin,processors));

  

  return 0;
}
