#include "Schedule.hpp"
#include "XMLSchedule.hpp"

#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace pelib;
using namespace std;

/*
void Schedule::dump(std::ostream& os, const TaskgraphRecord& tgr, const Record& amploutput)
{
  
  vector<string> taskids = tgr.get_taskids();
  float target_makespan = amploutput.find<Scalar <float> >("M")->getValue();
  int processors = amploutput.find<Scalar<int> >("p")->getValue();
  auto frequencies = amploutput.find<Vector<int, int> >("frequency")->getValues();
  std::map<int, std::map<int, int> > full_schedule = amploutput.find<Matrix <int, int, int> >("schedule")->getValues(); 
  
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
      std::map<int, int> procschedule = full_schedule.find(p+1)->second;

      for(int i = 0; (unsigned)i < procschedule.size(); i++)
	{
	  int id = procschedule.find(i)->second;
	  maxid = max(maxid,id);
	  string taskid;
	 
	if(id > 0)
	{ 
	  // Bounds check. Will throw if there are more (higher numbered) tasks in amploutput schedule than in taskgraphrecord.
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
	}
	}
      ss << " </core>\n";
    }
  ss << "</schedule>\n";

  // Final sanity check.
  if((unsigned)maxid < taskids.size())
    {
      cerr << "Warning: Not all tasks in task graph have been scheduled.\n"; //TODO: Warning or error?
    }
  os << ss.str() << endl;
}
*/

void
XMLSchedule::dump(ostream& os, const StreamingAppData *sched) const
{
	//sr.theSchedule->get_document()->write_to_stream(os);
}

Schedule*
XMLSchedule::parse(istream &is)
{
/*
  using namespace xmlpp;
  DomParser* parser = new DomParser();
  parser->parse_stream(is);

  return ScheduleRecord(parser);
*/
	return new Schedule();
}

XMLSchedule*
XMLSchedule::clone() const
{
	return new XMLSchedule();
}
