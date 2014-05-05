#include "TaskgraphRecord.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include <Record.hpp>
#include <AmplDataParser.hpp>
#include <Data.hpp>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Scalar.hpp"
#include <muParser.h>
extern "C"{
#include <igraph.h>
}
using namespace std;
#define VERY_SMALL 1e-6
namespace pelib
{	

  //used only as a temporary storage locally
  struct Vertex_info {
    std::string taskname;
    std::string taskid;
    double workload;
    int max_width; 
    std::string efficiency_line;
    double target_makespan;
    bool operator <(const Vertex_info &other) const
    {
      return taskid < other.taskid;
    }
    bool operator==(const std::string &other_taskid) const
    {
      return taskid == other_taskid;
    }
  };
  
   
  TaskgraphRecord::TaskgraphRecord(igraph_t* graph) : graph(graph){}

  TaskgraphRecord::~TaskgraphRecord()
  {
    igraph_destroy(graph);
    delete graph;
    if(architecture != nullptr)
      {
	delete architecture;
      }
  }

  TaskgraphRecord::TaskgraphRecord(const TaskgraphRecord &rhs)
  {
    *this = rhs;
  }
  
  
  TaskgraphRecord&
  TaskgraphRecord::operator=(const TaskgraphRecord &rhs)
  {
    graph = new igraph_t(*rhs.graph);
    return *this;
  }
  
  TaskgraphRecord::TaskgraphRecord(const Record& record)
  {
// TODO, call TaskgraphRecord(const TaskgraphRecord& tgr, const Record& record);
//  with emty graph
     throw runtime_error("Not implemented");
  }
  TaskgraphRecord::TaskgraphRecord(const TaskgraphRecord& tgr, const Record& record)
  {
    architecture = new Record(*tgr.architecture);
    graph = new igraph_t();
    igraph_copy(graph,tgr.graph);
    vector<Vertex_info> tasks = buildVertexVector();

    
    auto e = record.find<Matrix<int, int, float> >("e");
    if(e == nullptr)
      {
	throw runtime_error("Record does not have neccesary data!\n");
      }
    auto e_matrix = e->getValues();
    if(e_matrix.size() != tasks.size())
      {
	throw runtime_error("Tasks size of record and taskgraph differ!\n");
      }

   

    for(size_t i = 1; i <= e_matrix.size(); i++)
      {
    stringstream ss;
    ss << std::setprecision(6)
       << std::setiosflags(std::ios::fixed)
       << std::setiosflags(std::ios::showpoint);
 
	auto row = (*e_matrix.find(i)).second;
	for(size_t j = 1; j <= row.size(); j++)
	  {
	    ss << (*row.find(j)).second << " ";
	  }
	tasks[i-1].efficiency_line = ss.str();
	tasks[i-1].target_makespan = VERY_SMALL; // STUB
      }

    //TODO: set proper value for target makespan here;

    string s1 = "efficiency";
    string s2 = "target_makespan";
    for(int i = 0; i < igraph_vcount(graph); i++)
      {
	SETVAS(graph,s1.c_str(),i,tasks[i].efficiency_line.c_str());
	SETVAN(graph,s2.c_str(),i,tasks[i].target_makespan);
      }
  }



  vector<Vertex_info> TaskgraphRecord::buildVertexVector() const
  {
    vector<Vertex_info> tasks;
    
    
    for(int id=0; id < igraph_vcount(graph);id++)
      {
	Vertex_info task;
	//workaround, igraph does not read default values.
	task.taskid = strcmp(VAS(graph,"taskid",id),"") != 0 ? VAS(graph,"taskid",id) : "UNNAMED_TASKID" ;
	task.taskname = strcmp(VAS(graph,"taskname",id),"") != 0 ? VAS(graph,"taskname",id) : "UNNAMED_TASKNAME" ;
	task.workload = !isnan((float)VAN(graph,"workload",id)) ? VAN(graph,"workload",id): 1.0;
	task.max_width = (int)VAN(graph,"max_width",id) != INT_MIN ?  VAN(graph,"max_width",id) : 1;
	task.efficiency_line = strcmp(VAS(graph,"efficiency",id),"") != 0 ? VAS(graph,"efficiency",id) : "fml:p == 1 ? 1 : 1e-6" ;
	
	tasks.push_back(task);
      }
    

    //Sort on taskid
    sort(tasks.begin(),tasks.end());
    


    return tasks;
  }
  Record  TaskgraphRecord::toRecord() const
  {

    size_t processors = 1; // Default
    if(architecture != nullptr)
      {
	auto p = architecture->find<Scalar<int> >("p");
	if(p == nullptr)
	  {
	    cerr << "Warning: Architecture exists but is missing information\n";
	  }
	else
	  {
	    processors = p->getValue();
	  }
      }

  // extract all task information to be able to sort it together

    vector<Vertex_info> tasks = buildVertexVector();
    //vector<int> ids;
    
    map<int, map<int, float> > efficiency_matrix;
    map<int, float> workloads;
    map<int, int> max_width;
    
    int id = 1;
    // Build the vectors and matrices, in the correct order
    for(auto i = tasks.begin(); i != tasks.end(); ++i, ++id)
      {
	//ids.push_back(id);
	workloads.insert(pair<int,float>(id,(*i).workload));
	max_width.insert(pair<int,int>(id,(*i).max_width));
	
	// Calculate the efficiency table from formula if such exists
	// The formula is kept in the graph
	map<int,float> row;
	if((*i).efficiency_line.substr(0,4) == "fml:")
	  {
	    using namespace mu;
	    double p;
	    string fml = (*i).efficiency_line.substr(4);
	    
	    Parser mathparser;
	    mathparser.SetExpr(fml);
	    // This is where you may define additional parameters
	    // for the efficiency function.
	    // It is allowed to define parameters not present in the
	    // formula, it will just be ignored.
	    mathparser.DefineVar("p",&p); 
	    
	    for(p = 1; p <= processors; p +=1)
	      {
		row.insert(pair<int,float>((int)p,mathparser.Eval()));
	      }
	  }
	else{
	  stringstream stream((*i).efficiency_line);

	  //vector<int> arr;
	  float num;
	  size_t count = 0;
	  size_t x = 1;
	  while(stream >>num && count++ <= processors)
	    {
	      row.insert(pair<int,float>(x,num));
	      ++x;
	    }

	  for(;count < processors; ++count, ++x)
	    {
	      row.insert(pair<int,float>(x,VERY_SMALL));
	    }
	}
	efficiency_matrix.insert(pair<int, map<int, float> >(id,row));
      }
    

    pelib::Vector<int,int> Wi("Wi",max_width);
    pelib::Vector<int,float> Tau("Tau",workloads);
    pelib::Matrix<int, int, float> e("e",efficiency_matrix);
    pelib::Scalar<int> n("n",tasks.size());
    //pelib::Scalar<vector<Vertex_info> > r_tasks("tasks",tasks);
    
    Record record;
    record.insert(&Wi);
    record.insert(&Tau);
    record.insert(&e);
    record.insert(&n);
    //record.insert(&r_tasks);

    return record;
  }

  void TaskgraphRecord::setArchitecture(const Record& architecture)
  {
    this->architecture = new Record(architecture);
  }


  std::vector<std::string> TaskgraphRecord::get_taskids() const
  {
    vector<string> taskids;
    for(int i = 0; i < igraph_vcount(graph); i++)
      {
	string tid = VAS(graph,"taskid",i);
	if(tid.compare("") == 0)
	  {
	    tid = "UNNAMED_TASKID"; //this is bad. failure?
	  }
	taskids.push_back(tid);
      }
    sort(taskids.begin(),taskids.end());
    return taskids;
  }
  float TaskgraphRecord::get_target_makespan() const
  {
    float target_makespan = GAN(graph,"target_makespan");
    return isnan(target_makespan) ? VERY_SMALL : target_makespan;
  }
  const char* TaskgraphRecord::get_autname() const
  {
    return GAS(graph,"autname");
  }

}

