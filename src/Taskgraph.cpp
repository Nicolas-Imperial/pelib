#include "Taskgraph.hpp"
#include "Vector.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/graph/graphml.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <sstream>

using namespace pelib;
using namespace std;

struct Vertex_info {
  std::string taskname;
  std::string taskid;
  double workload;
  int max_width; // should be unsigned but boost can not cast from int
  std::string efficiency_bool;
  bool operator <(const Vertex_info &other) const
  {
    return taskid < other.taskid;
  }
  bool operator==(const std::string &other_taskid) const
  {
    return taskid == other_taskid;
  }
};

Record Taskgraph::parse(istream& data){
  using namespace boost;
  Record record;
  std::ifstream f;
  typedef adjacency_list<vecS, vecS, directedS,Vertex_info> BoostGraphType;
  typedef dynamic_properties BoostDynamicProperties;
  
  BoostGraphType graph;
  BoostDynamicProperties dp;
  dp.property("taskname",get(&Vertex_info::taskname,graph));
  dp.property("taskid",get(&Vertex_info::taskid,graph));
  dp.property("workload",get(&Vertex_info::workload,graph));
  dp.property("max_width",get(&Vertex_info::max_width,graph));
  dp.property("efficiency_bool",get(&Vertex_info::efficiency_bool,graph));

  
  read_graphml(data,graph,dp);


  // Read all task information into vector
  auto *tasks = new vector<Vertex_info>();
  for(auto taski = vertices(graph); taski.first != taski.second; ++taski.first)
    {
      Vertex_info *task = new Vertex_info();
      task->taskname = get(&Vertex_info::taskname,graph,*taski.first);
      task->taskid = get(&Vertex_info::taskid,graph,*taski.first);
      task->workload = get(&Vertex_info::workload,graph,*taski.first);
      task->max_width = get(&Vertex_info::max_width,graph,*taski.first);
      task->efficiency_bool = get(&Vertex_info::efficiency_bool,graph,*taski.first);
      tasks->push_back(*task);
      //cout << "taskname: " << get(&Vertex_info::taskname,graph,*taski.first)  << "\t";
      //cout << "taskid: " << get(&Vertex_info::taskid,graph,*taski.first)  << "\t";
      //cout << "workload: " << get(&Vertex_info::workload,graph,*taski.first) << "\n";
     
    }
  vector<pair<string,string> > channels;
  //compile a list all channels
  for(auto channeli = edges(graph); channeli.first != channeli.second; ++channeli.first)
    {
      auto s = source(*channeli.first,graph);
      auto d = target(*channeli.first,graph);
      channels.push_back(pair<string,string>(get(&Vertex_info::taskid,graph,s),get(&Vertex_info::taskid,graph,d)));
    }
  
  //Sort on taskid (string, unique)
  sort(tasks->begin(),tasks->end());
  //copy(tasks->begin(),tasks->end(),ostream_iterator<Vertex_info>(cout, " "));
  cout << "\nSorted: ";
  vector<int> ids;
  vector<vector<int> > efficiency_matrix;
  //vector<int> workloads;
  map<int, float> workloads;
  map<int, int> max_width;
  //vector<int> max_width;
  int id = 1;
  // Build the vectors and matrices, in the correct order
  for(auto i = tasks->begin(); i != tasks->end(); ++i, ++id)
    {
      ids.push_back(id);
      //workloads.push_back((*i).workload);
      workloads.insert(pair<int,float>(id,(*i).workload));
      //max_width.push_back((*i).max_width);
      max_width.insert(pair<int,int>(id,(*i).max_width));
      stringstream stream((*i).efficiency_bool);
      vector<int> arr;
      int num;
      int count = 0;
      while(stream >>num)
	{
	  arr.push_back(num);
	  ++count;
	}
      for(;count <= (*i).max_width; ++count)
	{
	  arr.push_back(0); 
	}
      efficiency_matrix.push_back(arr);

      //(i).edge_iterator();
      
    }

  // Build a channel tuple vector by correlating taskids with its assigned number.
  // Yes this is slow. This is the only O(n2) part in the parser.
  vector< pair<int, int> > channels_id;
  for(auto channelsi = channels.begin(); channelsi != channels.end(); ++channelsi)
    {
      int sourceid = (find(tasks->begin(), tasks->end(),(*channelsi).first) - tasks->begin()) + 1;
      int destid = (find(tasks->begin(), tasks->end(),(*channelsi).second) - tasks->begin()) + 1;
      channels_id.push_back(pair<int,int>(sourceid,destid));
    }
									     
  //copy(ids.begin(),ids.end(),ostream_iterator<int>(cout, " "));
  pelib::Vector<int,int> Wi("Wi",max_width);
  pelib::Vector<int,float> Tau("Tau",workloads);
  //record.add("tau",workload)
  //record.add("e",efficiency_matrix);
  record.insert(&Wi);
  record.insert(&Tau);
  //record.add("taskgraph",tasks);
  //record.add("links",channels_id)

  std::cout << "Done\n";

  return record;
}






