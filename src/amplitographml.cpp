#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include <AmplInput.hpp>
#include <GraphML.hpp>

int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;

  GraphML tg; 
  AmplInput ai(AmplInput::floatHandlers());

  std::cout << std::setprecision(6)                                                                                                        
	    << std::setiosflags(std::ios::fixed)                                                                                                     
	    << std::setiosflags(std::ios::showpoint);

  tg.dump(cout, Taskgraph(ai.parse(cin)));

  return 0;
}
