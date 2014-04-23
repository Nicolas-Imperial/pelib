#include "Taskgraph.hpp"

#include "Vector.hpp"
#include "Matrix.hpp"

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

Record Taskgraph::parse(istream& data)
{
  return parse(data,128);

}
Record Taskgraph::parse(istream& data, size_t processors){
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
  dp.property("efficiency",get(&Vertex_info::efficiency_line,graph));

  
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
      task->efficiency_line = get(&Vertex_info::efficiency_line,graph,*taski.first);
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
  
  //Sort on taskid
  sort(tasks->begin(),tasks->end());



  vector<int> ids;

  map<int, map<int, float> > efficiency_matrix;
  map<int, float> workloads;
  map<int, int> max_width;

  int id = 1;
  // Build the vectors and matrices, in the correct order
  for(auto i = tasks->begin(); i != tasks->end(); ++i, ++id)
    {
      ids.push_back(id);
      workloads.insert(pair<int,float>(id,(*i).workload));
      max_width.insert(pair<int,int>(id,(*i).max_width));

      stringstream stream((*i).efficiency_line);
      map<int,float> row;
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

      efficiency_matrix.insert(pair<int, map<int, float> >(id,row));
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
  pelib::Matrix<int, int, float> e("e",efficiency_matrix);
  pelib::Scalar<int> n("n",tasks->size());
  //record.add("e",efficiency_matrix);
  record.insert(&Wi);
  record.insert(&Tau);
  record.insert(&e);
  record.insert(&n);
  //record.add("taskgraph",tasks); //do this, but later
  //record.add("links",channels_id);

  std::cout << "Done\n";

  return record;
}






