#include <iostream>
#include <fstream>
#include <AmplInput.hpp>
#include <sstream>
#include "GraphML.hpp"
int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;

  if(argc < 2)
    {
      cerr << "usage: cat takgraph.graphml | " << argv[0] << " <architecture.dat>";
      return 0;
    }

  AmplInput archparser;
  ifstream ifs(argv[1]);

  Algebra arch = archparser.parse(ifs);

  GraphML taskgparser;
  TaskgraphRecord taskgraph = taskgparser.parse(cin);

  Algebra record = taskgraph.toAlgebra(arch);

  cout << std::setprecision(6) 
       << std::setiosflags(std::ios::fixed)
       << std::setiosflags(std::ios::showpoint);

  TaskgraphRecord tgr(taskgraph, record);
  taskgparser.dump(cout, tgr);
  
  return 0;
}
