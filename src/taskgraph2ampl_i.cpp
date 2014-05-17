#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include "IGraph.hpp"

int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;

  Record architecture;
  IGraph input;

  AmplInput output;
  AmplInput ai;
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
  
  // Set floating point var output format to fixed at 7 digits
  std::cout << std::setprecision(6)                                                                                                        
	    << std::setiosflags(std::ios::fixed)                                                                                                     
	    << std::setiosflags(std::ios::showpoint);  

  TaskgraphRecord tgr(input.parse(cin));
  tgr.setArchitecture(architecture);
  output.dump(cout,tgr);  

  return 0;
}
