#include <iostream>
#include <fstream>
#include <AmplInput.hpp>
#include "Taskgraph.hpp"
int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;
  size_t processors = 128;
  if(argc < 2)
    {
      cerr << "usage: cat takgraph.graphml | " << argv[0] << " <p8.txt>";
      return 0;
    }

  AmplInput archparser;
  ifstream ifs(argv[1]);
  //istream &is = ifs;
		 //  Record arch = archparser.parse);
  
  Taskgraph taskgparser;
  Record taskgraph = taskgparser.parse(cin);
  return 0;
}
