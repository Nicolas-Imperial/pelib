#include "Schedule.hpp"
#include "Scalar.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#include <exception>
#include <vector>
#include <sstream>
using namespace pelib;
using namespace std;

void Schedule::dump(std::ostream& os,const TaskgraphRecord& tgr,const Record& amploutput)
{
  
  vector<string> taskids = tgr.get_taskids();
  float target_makespan = tgr.get_target_makespan();
  int processors = amploutput.find<Scalar<int> >("p")->getValue();
  auto frequencies = amploutput.find<Vector<int, int> >("frequency")->getValues();
  auto full_schedule = amploutput.find<Matrix <int, int, int> >("schedule")->getValues();
 
  
  //TODO: where should schedule name come from?
  stringstream ss;
  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
     << "<schedule name=\"schedule\" autname=\"" << tgr.get_autname() << "\" "
     << "cores=\"" << processors << "\" "
     << "tasks=\"" << taskids.size() << "\" "
     << "roundtime=\"" << target_makespan << "\">\n";
    


  int maxid = 0; // for consistency check
  //assuming p should start from zero, even though ampl always start with one
  for(int p = 0; p < processors; p++)
    {
      ss << " <core coreid=\"" << p << "\">\n";
      auto procschedule = full_schedule.find(p+1)->second;

      for(int i = 0; (unsigned)i < procschedule.size(); i++)
	{
	  int id = procschedule.find(i)->second;
	  maxid = max(maxid,id);
	  string taskid;
	  
	  // Bounds check. Will throw if there are more tasks in amploutput than in taskgraphrecord.
	  try
	    {
	      //taskid = taskids[id-1];
	      taskid = taskids.at(id-1); // With bounds check
	    }
	  catch (std::out_of_range e)
	    {
	      cerr << "Error: Missmatch, ampl_output task id " << id << " does not exist in the task graph\n";
	      throw e;
	    }

	  if(id == 0) // 0 = no more tasks scheduled on this core
	    {
	      break;
	    }
	  ss << "   <task taskid=\"" << taskid << "\" ordering=\"" << i << "\" "
	     << "frequency=\"" << frequencies.find(id)->second <<  "\" />\n";
	  //cout << taskids[id - 1] << " ";

	}
      ss << " </core>\n";
      //cout << endl;
    }
  ss << "</schedule>\n";

  // Final sanity check.
  if((unsigned)maxid < taskids.size())
    {
      cerr << "Warning: Not all tasks in task graph have been scheduled.\n"; //TODO: Warning or error?
    }
  os << endl << ss.str() << endl;
  

}
