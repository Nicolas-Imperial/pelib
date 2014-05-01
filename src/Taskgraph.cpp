#include "Taskgraph.hpp"

#include "Vector.hpp"
#include "Matrix.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/graph/graphml.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <muParser.h>
#include <vector>
#include <sstream>
extern "C"{
#include <igraph.h>
}
using namespace pelib;
using namespace std;
#include <boost/math/special_functions/fpclassify.hpp> // isnan
#include <cmath>
#include <climits>

struct Vertex_info {
  std::string taskname;
  std::string taskid;
  double workload;
  int max_width; // should be unsigned but boost can not cast from int
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

struct Graph_info{
  string autname;
  float target_makespan;
};

struct Edge_info{}; //dummy

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,Vertex_info,boost::no_property,Graph_info> BoostGraphType;
typedef boost::dynamic_properties BoostDynamicProperties;

Record Taskgraph::parse(istream& data)
{
  return parse(data,128);

}


BoostDynamicProperties make_dp(BoostGraphType& graph)
{
  using namespace boost;
  BoostDynamicProperties dp;
  dp.property("taskname",get(&Vertex_info::taskname,graph));
  dp.property("taskid",get(&Vertex_info::taskid,graph));
  dp.property("workload",get(&Vertex_info::workload,graph));
  dp.property("max_width",get(&Vertex_info::max_width,graph));
  dp.property("efficiency",get(&Vertex_info::efficiency_line,graph));

  
  //topology[boost::graph_bundle].myName = get(graphname_map,"graphname");

  //dp.property("autname",get(&Graph_info::autname,graph));
  //dp.property("target_makespan",get(&Graph_info::target_makespan,graph));
  return dp;
}

Record Taskgraph::parse(istream& data, size_t processors)
{
  
  Record record;
  igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
  igraph_t the_graph;

  //create a FILE* from istream by building a posix pipe. This wont work in windows...
  int p[2];
  pipe(p);
  
  FILE *fake_fileptr = fdopen(p[0], "r"); 
  std::string str;
  char c;
  c = cin.get();
  while(!cin.eof())
    {
      write(p[1],&c,1);
      c = cin.get();
    }
  close(p[1]);
  igraph_read_graph_graphml(&the_graph,fake_fileptr,0);  
  close(p[0]);
   
  //  cout << "Name: " <<   igraph_cattribute_GAS(&the_graph,"autname") << endl;
  // extract all task information to be able to sort it together
  vector<Vertex_info> tasks;
  for(int id=0; id < igraph_vcount(&the_graph);id++)
    {
      Vertex_info task;
      //workaround, igraph does not read default values.
      task.taskid = strcmp(VAS(&the_graph,"taskid",id),"") != 0 ? VAS(&the_graph,"taskid",id) : "UNNAMED_TASKID" ;
      task.taskname = strcmp(VAS(&the_graph,"taskname",id),"") != 0 ? VAS(&the_graph,"taskname",id) : "UNNAMED_TASKNAME" ;
      task.workload = !isnan((float)VAN(&the_graph,"workload",id)) ? VAN(&the_graph,"workload",id): 1.0;
      task.max_width = (int)VAN(&the_graph,"max_width",id) != INT_MIN ?  VAN(&the_graph,"max_width",id) : 1;
      task.efficiency_line = strcmp(VAS(&the_graph,"efficiency",id),"") != 0 ? VAS(&the_graph,"efficiency",id) : "fml:p == 1 ? 1 : 1e-6" ;
      
      tasks.push_back(task);
    }

  /*
  vector<pair<string,string> > channels;
  //compile a list all channels
  for(auto channeli = edges(graph); channeli.first != channeli.second; ++channeli.first)
    {
      auto s = source(*channeli.first,graph);
      auto d = target(*channeli.first,graph);
      channels.push_back(pair<string,string>(get(&Vertex_info::taskid,graph,s),get(&Vertex_info::taskid,graph,d)));
    }
  */

  //Sort on taskid
  sort(tasks.begin(),tasks.end());


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
	while(stream >>num)
	  {
	    row.insert(pair<int,float>(x,num));
	    //arr.push_back(num);
	    ++count;
	    ++x;
	  }
#define VERY_SMALL 1e-6
	for(;count < processors; ++count, ++x)
	  {
	    row.insert(pair<int,float>(x,VERY_SMALL));
	  }
      }
      efficiency_matrix.insert(pair<int, map<int, float> >(id,row));
    }
    
  /*
  // Build a channel tuple vector by correlating taskids with its assigned number.
  // Yes this is slow. This is the only O(n2) part in the parser.
  vector< pair<int, int> > channels_id;
  for(auto channelsi = channels.begin(); channelsi != channels.end(); ++channelsi)
    {
      int sourceid = (find(tasks.begin(), tasks.end(),(*channelsi).first) - tasks.begin()) + 1;
      int destid = (find(tasks.begin(), tasks.end(),(*channelsi).second) - tasks.begin()) + 1;
      channels_id.push_back(pair<int,int>(sourceid,destid));
    }*/
									     
  pelib::Vector<int,int> Wi("Wi",max_width);
  pelib::Vector<int,float> Tau("Tau",workloads);
  pelib::Matrix<int, int, float> e("e",efficiency_matrix);
  pelib::Scalar<int> n("n",tasks.size());
  pelib::Scalar<vector<Vertex_info> > r_tasks("tasks",tasks);
  // yes, this could have been a pelib::Vector. But we do not want anyone else to dump this entry
  //pelib::Scalar<vector< pair<int, int> > > r_channels("channels",channels_id); 
  pelib::Scalar<igraph_t> r_graph("graph",the_graph);
 
  record.insert(&Wi);
  record.insert(&Tau);
  record.insert(&e);
  record.insert(&n);
  record.insert(&r_tasks);
  //record.insert(&r_channels);
  record.insert(&r_graph);

  
  
  return record;
}

void Taskgraph::dump(ostream& o, const Record& record) const {

  int p[2];
  pipe(p);
  
  FILE *fake_fileptr = fdopen(p[1], "w"); 

  auto r_graph = record.find<Scalar<igraph_t> >("graph");
  if(r_graph == nullptr)
    {
      throw runtime_error("hello");
      cerr << "Dump failure: Can not dump this type as taskgraph\n";
      return;
    }

  auto &the_graph = r_graph->getValue();
  igraph_write_graph_graphml(&the_graph,fake_fileptr,true); 
  close(p[1]);
  


  FILE *instream = fdopen (p[0], "r");
  char c;
  while ((c = fgetc (instream)) != EOF)
    {
      cout << c;
    }
  fclose (instream);
  close(p[0]);
  /*
  auto r_graph =(record.find<Scalar<BoostGraphType> >("graph"));
  if(r_graph == nullptr)
    {
      cerr << "Dump failure: Can not dump this type as taskgraph\n";
      return;
    }
  auto graph = r_graph->getValue(); //what is the value of a autograph .. =)
  write_graphml(o, graph, make_dp(graph),true);
*/
}; 

void Taskgraph::duplicate_tasks(Record& record, const vector<int>& to_duplicate)
{
  auto r_graph =(record.find<Scalar<BoostGraphType> >("graph"));
  if(r_graph == nullptr)
    {
      cerr << "Error: Input is not a taskgraph\n";
      return;
    }

  // I am assuming the rest of the data exists. 
  auto graph = r_graph->getValue();
  auto tasks =    record.find<Scalar<vector< Vertex_info    > > >("tasks")->getValue();
  auto channels = record.find<Scalar<vector< pair<int, int> > > >("channels")->getValue();
 

 
  int num = 1;
  for(auto it = to_duplicate.begin(); it != to_duplicate.end(); it++)
    {
      auto task = tasks[*it-1];
      const string original_taskid = task.taskid;
      task.taskid = task.taskid + "_" + to_string(num++);
      auto updated_channels(channels);
      
      auto  original = *vertices(graph).first;
      bool found = false;
      for(auto taski = vertices(graph); taski.first != taski.second; ++taski.first) //todo: get rid of this loop
	{
	  if( get(&Vertex_info::taskid,graph,*taski.first) == original_taskid)
	    {
	      original = *taski.first;
	      found = true;
	      break;
	    }
	}
      if(!found) throw ":-(";
      
      auto new_task = add_vertex(task,graph);
      
      
      for(auto channeli = edges(graph); channeli.first != channeli.second; ++channeli.first)
	{
	  auto s = source(*channeli.first,graph);
	  auto d = target(*channeli.first,graph);
	  if(s == original)
	    {
	      add_edge(new_task,d,graph);
	    }
	  else if(d == original)
	    {
	      add_edge(s,new_task,graph);
	    }
	}
    }
  
  Scalar<BoostGraphType> tmp("graph",graph);
  record.insert(&tmp);
  stringstream s;
  this->dump(s,record);
  record = this->parse(s);
  //record = parse(this->dump(record));
}

	


