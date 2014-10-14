#include <iostream>
#include <fstream>
#include <sstream>

#include <AmplPlatform.hpp>
#include <GraphML.hpp>

int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;

  if(argc < 2)
    {
      cerr << "usage: cat takgraph.graphml | " << argv[0] << " <platform.dat>";
      return 0;
    }

  AmplPlatform archparser;
  ifstream ifs(argv[1]);

  Platform *arch = archparser.parse(ifs);

  GraphML taskgparser;
  Taskgraph *taskgraph = taskgparser.parse(cin);

  cout << std::setprecision(6) 
       << std::setiosflags(std::ios::fixed)
       << std::setiosflags(std::ios::showpoint);

  taskgparser.dump(cout, taskgraph, arch);
  
  return 0;
}
