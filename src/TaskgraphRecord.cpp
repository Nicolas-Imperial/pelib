#include "TaskgraphRecord.hpp"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include <AmplOutput.hpp>
#include <Algebra.hpp>
#include <AmplDataParser.hpp>
#include <Data.hpp>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Scalar.hpp"
#include "Set.hpp"
#include "MakespanCalculator.hpp"
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
    int id;
    std::string taskname;
    std::string taskid;
    double workload;
    int max_width; 
    std::string efficiency_line;
    bool operator <(const Vertex_info &other) const
    {
      return taskid < other.taskid;
    }
    bool operator==(const std::string &other_taskid) const
    {
      return taskid == other_taskid;
    }
  };
  
   
  TaskgraphRecord::TaskgraphRecord(igraph_t* graph) : graph(graph)
  {
	//architecture = nullptr;
  }

  TaskgraphRecord::~TaskgraphRecord()
  {
    igraph_destroy(graph);
    delete graph;

	/*
    if(architecture != nullptr)
      {
	delete architecture;
      }
	*/
  }

  TaskgraphRecord::TaskgraphRecord(const TaskgraphRecord &rhs)
  {
    *this = rhs;
  } 
  
  TaskgraphRecord&
  TaskgraphRecord::operator=(const TaskgraphRecord &rhs)
  {
    graph = new igraph_t();
    igraph_copy(graph, rhs.graph);
    return *this;
  }
  
  TaskgraphRecord::TaskgraphRecord(const Algebra& record)
  {
    igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
    graph = new igraph_t();
    //architecture = 0;
    FILE* f = fopen("src/empty_taskgraph.graphml","r"); //TODO: include in ar? 
    if(!f)
      {
	throw runtime_error("Error: can not find an empty taskgraph file to work from.");
      }

    if(igraph_read_graph_graphml(graph,f,0))
      {
	throw runtime_error("Error: Failed loading empty taskgraph\n");
      }
    fclose(f);
    
    int n = record.find<Scalar<int> >("n")->getValue();
    igraph_add_vertices(graph,n,0);
    SETGAS(graph,"autname","GENERATED_FROM_RECORD");
    SETGAS(graph,"target_makespan", "fml:synthetic");

    int i;
    for(i = 0; i < n; i++)
      {
	//ensure correct sorting of numbers by inserting extra "_" which is sorted as larger than 0-9
	const int num_pads = (int) log10(i+1) + 1;

	stringstream taskname;
	stringstream taskid;
	taskname << "task" << string(num_pads,'_') << (i+1) << "_name";
	taskid   << "task" << string(num_pads,'_') << (i+1) << "_id";
	SETVAS(graph,"taskname",i,taskname.str().c_str());
	SETVAS(graph,"taskid",i,taskid.str().c_str());
      }
    merge_taskgraph_record(record);
  }
	 
  TaskgraphRecord::TaskgraphRecord(const TaskgraphRecord& tgr, const Algebra& record)
  {
    igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
    //architecture = new Algebra(*tgr.architecture); //TODO: call clone() ?
    graph = new igraph_t();
    igraph_copy(graph,tgr.graph);    
    merge_taskgraph_record(record);
  }

  void TaskgraphRecord::merge_taskgraph_record(const Algebra& record)
  {

    vector<Vertex_info> tasks = buildVertexVector();
    

    auto e = record.find<Matrix<int, int, float> >("e");
    if(e == nullptr)
      {
	throw runtime_error("Algebra does not have neccesary data!\n");
      }
    auto e_matrix = e->getValues();
    if(e_matrix.size() != tasks.size())
      {
	throw runtime_error("Tasks size of record and taskgraph differ!\n");
      }
    
    
    //build efficiency lines
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
      }

    //get max width
    auto wi = record.find<Vector<int,int> > ("Wi");
    if(wi)
      {
	for(int i = 0; i < igraph_vcount(graph); i++)
	  {
	    tasks[i].max_width = wi->find(i+1);
	  }
      }


    auto tm = record.find<Scalar<float> >("M");
    if(tm)
      {
	float target_makespan = tm->getValue();
	string s2 = "target_makespan";
	char mstring[32];
	sprintf(mstring,"%f",target_makespan);
	SETGAS(graph,s2.c_str(),mstring);
      }



    for(int i = 0; i < igraph_vcount(graph); i++)
      {
	string s1 = "efficiency";
	SETVAS(graph,s1.c_str(),i,tasks[i].efficiency_line.c_str());
	SETVAN(graph,"max_width",i,tasks[i].max_width);
      }
	
  }

  vector<Vertex_info> TaskgraphRecord::buildVertexVector() const
  {
    vector<Vertex_info> tasks;
    
    for(int id=0; id < igraph_vcount(graph);id++)
      {
	Vertex_info task;
	task.id = id;
	//workaround, igraph does not read default values.
	task.taskid = strcmp(VAS(graph,"taskid",id),"") != 0 ? VAS(graph,"taskid",id) : "UNNAMED_TASKID" ;
	task.taskname = strcmp(VAS(graph,"taskname",id),"") != 0 ? VAS(graph,"taskname",id) : "UNNAMED_TASKNAME" ;
	task.workload = !isnan((float)VAN(graph,"workload",id)) ? VAN(graph,"workload",id): 1.0;
	task.max_width = (int)VAN(graph,"max_width",id) != INT_MIN ?  VAN(graph,"max_width",id) : 1;


	if (strcmp(VAS(graph, "efficiency", id), "") != 0)
	{
		task.efficiency_line = VAS(graph, "efficiency", id);
	}
	else
	{
//		if(architecture != nullptr)
//		{
			stringstream ss;
			ss << task.max_width;
			task.efficiency_line = string("fml:p <= ") + ss.str() + "? 1 : 1e-6";
//		}
	}
	
	tasks.push_back(task);
      }

    //Sort on taskid
    sort(tasks.begin(),tasks.end());

    return tasks;
  }

/*
  Record
  TaskgraphRecord::toRecord() const
  {
	if(architecture != nullptr)
	{
		return this->toRecord(*this->architecture);
	}
	else
	{
	    cerr << "Error: no architecture information" << endl;
	    abort();
	}
  }
*/

  Algebra
  TaskgraphRecord::toAlgebra(const Algebra &arch) const
  {
    size_t processors;
	const Scalar<int> *p = arch.find<Scalar<int> >("p");
	if(p == nullptr)
	  {
	    cerr << "Error: Cannot find scalar \"p\" in architecture record" << endl;
	    abort();
	  }
	else
	  {
	    processors = p->getValue();
	  }

    // extract all task information to be able to sort it together
    vector<Vertex_info> tasks = buildVertexVector();
    //vector<int> ids;
    
    map<int, map<int, float> > efficiency_matrix;
    map<int, int> workloads;
    map<int, int> max_width;
    
    int id = 1;
    // Build the vectors and matrices, in the correct order
    for(auto i = tasks.begin(); i != tasks.end(); ++i, ++id)
      {
	//ids.push_back(id);
	workloads.insert(pair<int,int>(id,(*i).workload));
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
	  while(stream >>num && count < processors)
	    {
	      row.insert(pair<int,float>(x,num));
	      ++x;
	      ++count;
	    }

	  for(;count < processors; ++count, ++x)
	    {
	      row.insert(pair<int,float>(x,VERY_SMALL));
	    }
	}
	efficiency_matrix.insert(pair<int, map<int, float> >(id,row));
      }
    

    pelib::Vector<int,int> Wi("Wi",max_width);
    pelib::Vector<int,int> Tau("Tau",workloads);
    pelib::Matrix<int, int, float> e("e",efficiency_matrix);
    pelib::Scalar<int> n("n",tasks.size());

    
    Algebra record;
    record.insert(&Wi);
    record.insert(&Tau);
    record.insert(&e);
    record.insert(&n);

    const MakespanCalculator*  msc = MakespanCalculator::getMakespanCalculator(GAS(graph,"target_makespan"));
    double target_makespan = msc->calculate(record, arch);

    delete msc;

    pelib::Scalar<float> M("M",(float)target_makespan);
    record.insert(&M);
    return record;
  }

/*
  void TaskgraphRecord::setArchitecture(const Algebra& architecture)
  {
    this->architecture = new Algebra(architecture);
  }
*/

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


  const char* TaskgraphRecord::get_autname() const
  {
    return GAS(graph,"autname");
  }


  double get_efficiency(const string& efficiency_line, int width)
  {
    using namespace mu;
    if (efficiency_line.substr(0,4).compare("fml:") == 0)
      {
	Parser mathparser;
	double p = width;
	mathparser.SetExpr(efficiency_line.substr(4));
	mathparser.DefineVar("p",&p);
	return mathparser.Eval();
      }
    else
      {
	//todo, turn into an array, return width'th value
	throw runtime_error("This should not happen. (Not an error, just not implemented yet");
      }
    return -1;
  }

/*
  float TaskgraphRecord::makespan_random(const vector<Vertex_info>& tasks )
  {
    //int n = architecture->find<Scalar<int> >("n")->getValue();
    int p = architecture->find<Scalar<int> >("p")->getValue();
    double min_freq = *architecture->find<Set<int> >("F")->getValues().begin();
    double max_freq = *architecture->find<Set<int> >("F")->getValues().end();
    
    double sum_pTw = 0;
    for(const Vertex_info& task : tasks)
      {
	int max_width = min(task.max_width,p);
	double efficiency = get_efficiency(task.efficiency_line,max_width);
	
	sum_pTw += task.workload / efficiency;
      }
    
    return (sum_pTw/max_freq + sum_pTw/min_freq) / 2;
  }
*/

  float
  TaskgraphRecord::getTargetMakespan(const Algebra& architecture) const
  {
	return this->toAlgebra(architecture).find<Scalar<float> >("M")->getValue();
  }
}

