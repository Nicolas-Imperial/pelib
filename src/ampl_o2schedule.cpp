#include "TaskgraphRecord.hpp"
#include "AmplOutput.hpp"
#include "IGraph.hpp"
#include "Schedule.hpp"
//#include "ScheduleRecord.hpp"

#include <fstream>


using namespace pelib;
using namespace std;

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      cerr << "Need taskgraph to perform this task\n";
      return 1;
    }

  IGraph tg;
  AmplOutput ao;
  Schedule sch;

  ifstream ifs(argv[1]);

  TaskgraphRecord tgr = tg.parse(ifs);
  ifs.close();
  
  Record amplschedule(ao.parse(cin));
  //cout << "The schedule:" << endl;
  //ao.dump(cout,amplschedule);

  sch.dump(cout, tgr, amplschedule);

  return 0;
}

  
