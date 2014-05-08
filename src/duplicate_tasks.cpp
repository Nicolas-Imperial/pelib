#include <vector>
#include <iostream>

#include "Taskgraph.hpp"
  using namespace std;
  using namespace pelib;

int main(int argc, char **argv)
{
  if(argc == 1) return 0; //nothing to duplicate
  // usage: duplicate_tasks [id0 id1 ...]
  Taskgraph input;
  auto data = input.parse(cin);
  

  vector<int> to_duplicate; //todo: add ensurecapacity argc -1;
  for(int i = 1; i < argc; i++)
    {
      to_duplicate.push_back(atoi(argv[i]));
    }
 
  //input.duplicate_tasks(data,to_duplicate); //todo:do

  input.dump(cout, data);
  return 0;
}
