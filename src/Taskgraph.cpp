#include "Taskgraph.hpp"
#include "TaskgraphRecord.hpp"

#include "Vector.hpp"
#include "Matrix.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <boost/graph/graphml.hpp>
//#include <boost/graph/adjacency_list.hpp>
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
#include <cstdio>



TaskgraphRecord Taskgraph::parse(istream& data)
{
  
  Record record;
  igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching

  igraph_t *the_graph = new igraph_t();

  //create a FILE* from istream by building a posix pipe. This wont work in windows...
  int p[2];

  pipe(p);

  FILE *fake_fileptr = fdopen(p[0], "r"); 
  char c;
  c = data.get();
  while(!data.eof())
    {
      write(p[1],&c,1);
      c = data.get();
    }
  close(p[1]);

  igraph_read_graph_graphml(the_graph,fake_fileptr,0);  
  
  close(p[0]);
   
  return TaskgraphRecord(the_graph);
}

void Taskgraph::dump(ostream& o, const TaskgraphRecord& record) const {
  int p[2];
  pipe(p);

  FILE *fake_fileptr = fdopen(p[1], "w"); 
  
  auto graph = record.graph; 
  igraph_write_graph_graphml(graph,fake_fileptr,true); 
  fclose(fake_fileptr);

  FILE *instream = fdopen (p[0], "r");
  char c;
  while ((c = fgetc (instream)) != EOF)
    {
      cout << c;
    }

  fclose (instream);
  close(p[0]);
  close(p[1]);
}; 


void Taskgraph::duplicate_tasks(TaskgraphRecord& record, const vector<int>& to_duplicate)
{
  //Implemented only for boost 
  throw runtime_error("Not implemented");
  return;
  /*
  auto r_graph =(record.find<Scalar<BoostGraphType> >("graph"));
  if(r_graph == nullptr)
    {
      throw runtime_error("Error: Input is not a taskgraph\n");
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
  */
}

	


