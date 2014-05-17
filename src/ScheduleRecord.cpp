#include "ScheduleRecord.hpp"

#include "Schedule.hpp"
#include "Scalar.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace pelib;
using namespace xmlpp;


ScheduleRecord::ScheduleRecord(xmlpp::DomParser* theSchedule) : theSchedule(theSchedule)
{}
  
ScheduleRecord::ScheduleRecord(const ScheduleRecord &rhs) : theSchedule(rhs.theSchedule)
{}


ScheduleRecord& ScheduleRecord::operator=(const ScheduleRecord &rhs)
{
  this->theSchedule = rhs.theSchedule;
  return *this;
}

    
ScheduleRecord::~ScheduleRecord()
{
  delete theSchedule;
}

// Internal use only
struct task_info
{
  string taskid;
  int ordering;
  int frequency;
};

Record ScheduleRecord::toRecord() const
{
  // Just like taskgraph conversion
  // this involves turning the data structure
  // inside out, and to convert from
  // the taskids to id 1..n, in a 
  // deterministic fashion.
  // The latter is slightly more complex
  // than taskgraph version
  // since the tasks may appear more than one
  // in the schedule.

  Element *root = theSchedule->get_document()->get_root_node();
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
      for(ordering = 0; ordering < one_core_schedule.size(); ordering++)
	{
	  int id = taskid2id.find(one_core_schedule[ordering].taskid)->second;
	  schedule_row.insert(pair<int,int>(ordering,id));
	  frequencies.insert(pair<int,int>(id,one_core_schedule[ordering].frequency));
	  //cout << "task id " << id << " taskid " <<  one_core_schedule[ordering].taskid << " ordering " << ordering << " frequency " << one_core_schedule[ordering].frequency << endl;
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
  return record;
}
    
