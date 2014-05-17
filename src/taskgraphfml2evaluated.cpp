#include <iostream>
#include <fstream>
#include <AmplInput.hpp>
#include <sstream>
#include "IGraph.hpp"
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

  Record arch = archparser.parse(ifs);

  IGraph taskgparser;
  TaskgraphRecord taskgraph = taskgparser.parse(cin);

  taskgraph.setArchitecture(arch);
  Record record = taskgraph.toRecord();

  cout << std::setprecision(6) 
       << std::setiosflags(std::ios::fixed)
       << std::setiosflags(std::ios::showpoint);

  TaskgraphRecord tgr(taskgraph,record);
  taskgparser.dump(cout, tgr);
  
  return 0;
}
