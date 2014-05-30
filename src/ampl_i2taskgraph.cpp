#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include "GraphML.hpp"

int main(int argc, char **argv)
{
#if 0
  using namespace std;
  using namespace pelib;

  GraphML tg;
  
  AmplInput ai;
  /*
  Record r = ai.parse(cin);
  cout << "This is the record\n";
  ai.dump(cout, r);
  cout << "Converting into taskgraph\n";
  TaskgraphRecord tgr(r);
  cout << "Succeeded without crashing" << endl;
  */
  std::cout << std::setprecision(6)                                                                                                        
	    << std::setiosflags(std::ios::fixed)                                                                                                     
	    << std::setiosflags(std::ios::showpoint);  

  tg.dump(cout, TaskgraphRecord(ai.parse(cin))); /*
  // Open input file
  std::ifstream myfile;
  myfile.open (argv[1], std::ios::in);
  tg.dump(cout, TaskgraphRecord(ai.parse(myfile)));
  myfile.close(); */
#endif

  return 0;
}
