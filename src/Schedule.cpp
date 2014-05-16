#include "Schedule.hpp"
#include "Scalar.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#include <exception>
#include <vector>
#include <sstream>
using namespace pelib;
using namespace std;

void Schedule::dump(std::ostream& os, const TaskgraphRecord& tgr, const Record& amploutput)
{
  
  vector<string> taskids = tgr.get_taskids();
  float target_makespan = tgr.getTargetMakespan();
  int processors = amploutput.find<Scalar<int> >("p")->getValue();
  auto frequencies = amploutput.find<Vector<int, int> >("frequency")->getValues();
  auto full_schedule = amploutput.find<Matrix <int, int, int> >("schedule")->getValues();
  if(taskids.size() != amploutput.find<Matrix <int, int, int> >("schedule")->getRowSize())
    {
      throw runtime_error("Data missmatch, different number of tasks\n");
    }

  
  //TODO: where should schedule name come from?
  stringstream ss;
  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
     << "<schedule name=\"schedule\" autname=\"" << tgr.get_autname() << "\" "
     << "cores=\"" << processors << "\" "
     << "tasks=\"" << taskids.size() << "\" "
     << "roundtime=\"" << target_makespan << "\">\n";
    



  //assuming p should start from zero, even though ampl always start with one
  for(int p = 0; p < processors; p++)
    {
      ss << " <core coreid=\"" << p << "\">\n";
      auto procschedule = full_schedule.find(p+1)->second;

      for(int i = 0; (unsigned)i < procschedule.size(); i++)
	{
	  int id = procschedule.find(i)->second;
	  if(id == 0) // 0 = no more tasks schuled on this core
	    {
	      break;
	    }
	  ss << "   <task taskid=\"" << taskids[id-1] << "\" ordering=\"" << i << "\" "
	     << "frequency=\"" << frequencies.find(id)->second <<  "\" />\n";
	  //cout << taskids[id - 1] << " ";

	}
      ss << " </core>\n";
      //cout << endl;
    }
  ss << "</schedule>\n";
  os << endl << ss.str() << endl;


}
