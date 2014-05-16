#include "Schedule.hpp"
#include "Scalar.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "AmplOutput.hpp" //for testing only

#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace pelib;
using namespace std;

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
	  //cout << taskids[id - 1] << " ";
	}
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
  os << ss.str() << endl;
}

struct task_info
{
  string taskid;
  int ordering;
  int frequency;
};

// Not functional as of now
void Schedule::parse(istream& is)
{
  using namespace xmlpp;
  DomParser parser;
  parser.parse_stream(is);
  Element *root = parser.get_document()->get_root_node();
  Node::NodeList processors = root->get_children();
  Node::NodeList::iterator iter;

  set<string> all_taskids;
  map<int, vector<task_info> > schedulemap;
  size_t max_schedule_size = 0;
  int n = stoi(root->get_attribute_value("tasks"));
  int p = stoi(root->get_attribute_value("cores"));
  float M = stof(root->get_attribute_value("roundtime"));
  for(iter = processors.begin(); iter != processors.end(); ++iter) //for each core
    {
      if((*iter)->get_name().compare("core")) //skip indentation characters et cetera
	{
	  continue;
	}
      
      int coreid = stoi( dynamic_cast<Element*>(*iter)->get_attribute_value("coreid"));
      vector<task_info> task_infos;

      auto tasks = (*iter)->get_children();
      for(auto taskiter = tasks.begin(); taskiter != tasks.end(); ++taskiter)
	{
	  //	  cout << (*taskiter)->get_name() << endl;
	  if((*taskiter)->get_name().compare("task")) //skip indentation characters et cetera
	    {
	      continue;
	    }

	  auto *task = dynamic_cast<Element*>(*taskiter);
	  
	  string taskid =    task->get_attribute_value("taskid");
	  string ordering =  task->get_attribute_value("ordering");
	  string frequency = task->get_attribute_value("frequency");
	  
	  all_taskids.insert(taskid);
	  task_info ti = {taskid,stoi(ordering),stoi(frequency)};
	  task_infos.push_back(ti);

	}
      max_schedule_size = max(task_infos.size(),max_schedule_size);

      sort(task_infos.begin(),task_infos.end(),[](const task_info& l,const task_info& r) { return l.ordering < r.ordering;});
      schedulemap.insert(pair<int,vector<task_info> >(coreid,task_infos));
      
    }

  
  vector<string> tmp(all_taskids.begin(),all_taskids.end());
  sort(tmp.begin(),tmp.end());
  map<string,int>taskid2id;
  for(size_t i = 0; i < tmp.size(); i++)
    {
      taskid2id.insert(pair<string,int>(tmp[i],i+1));
    }
  
  
  map<int, map<int, int> > sched;
  map<int,int> frequencies;
  for(int core = 0; core < p; core++)
    {
      map<int, int> schedule_row;
      auto one_core_schedule = schedulemap.find(core)->second;
      size_t ordering;
      cout << "core: " << core << endl;
      for(ordering = 0; ordering < one_core_schedule.size(); ordering++)
	{
	  int id = taskid2id.find(one_core_schedule[ordering].taskid)->second;
	  schedule_row.insert(pair<int,int>(ordering,id));
	  frequencies.insert(pair<int,int>(id,one_core_schedule[core].frequency));
	  cout << "task id " << id << "taskid " <<  one_core_schedule[core].taskid << " ordering " << ordering << " frequency " << one_core_schedule[core].frequency << endl;
	}
      // pad with 0
      for(; ordering < max_schedule_size; ordering++)
	{
	  schedule_row.insert(pair<int,int>(ordering,0));
	}
      
      sched.insert(pair<int, map<int,int> >(core+1,schedule_row));
    }
  Matrix<int,int,int> ampl_schedule("schedule",sched);
  Vector<int,int> ampl_frequencies("frequency",frequencies);
  Scalar<int> ampl_n("n",n);
  Scalar<int> ampl_p("p",p);
  Scalar<float> ampl_M("M",M);
  
  Record record;
  record.insert(&ampl_n);
  record.insert(&ampl_p);
  record.insert(&ampl_M);
  record.insert(&ampl_schedule);
  record.insert(&ampl_frequencies);
  AmplOutput ao;
  ao.dump(cout,record);
  
}

