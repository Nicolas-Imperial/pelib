#include <iostream>
#include <fstream>
#include <AmplInput.hpp>
#include <sstream>
#include "Taskgraph.hpp"
int main(int argc, char **argv)
{
  using namespace std;
  using namespace pelib;

  if(argc < 2)
    {
      cerr << "usage: cat takgraph.graphml | " << argv[0] << " <p8.txt>";
      return 0;
    }
  cout << "Ok, starting ...\n";
  AmplInput archparser;
  ifstream ifs(argv[1]);

  Record arch = archparser.parse(ifs);

  cout << "this is the arch\n";
  archparser.dump(cout,arch);
    

  Taskgraph taskgparser;

  cout << "parsing cin to taskgraph\n";
  TaskgraphRecord taskgraph = taskgparser.parse(cin);

  cout << "Setting architecture\n";
  taskgraph.setArchitecture(arch);

  cout << "This is the taskgraph:\n";
  taskgparser.dump(cout, taskgraph);

  cout << "converting from taskgraph to record\n";
  Record record = taskgraph.toRecord();

  cout << "the record: \n";
  cout << std::setprecision(6)                                                                         	      << std::setiosflags(std::ios::fixed)                                                            	      << std::setiosflags(std::ios::showpoint);

  archparser.dump(cout,record);




  cout << "going back to taskgraph\n";
  TaskgraphRecord tgr(taskgraph,record);
  cout << "dumping\n";
  taskgparser.dump(cout, tgr);
  cout << "Done\n";
  
  return 0;
}
