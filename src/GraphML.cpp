#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <thread>

extern "C"{
#include <igraph.h>
}

#include "Taskgraph.hpp"
#include "GraphML.hpp"

#include <Scalar.hpp>
#include <Vector.hpp>
#include <Matrix.hpp>
#include <Set.hpp>
#include <Task.hpp>

#include <CastException.hpp>
#include <ParseException.hpp>

#include <signal.h>

using namespace pelib;
using namespace std;
using namespace xmlpp;

void
GraphML::dump(ostream& os, const StreamingAppData *data, const Architecture *arch) const
{
	const Taskgraph *tg = dynamic_cast<const Taskgraph* >(data);
	if(tg == NULL) throw CastException("Parameter \"data\" was not of type \"Taskgraph*\".");

	igraph_t *graph = new igraph_t();
	igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching

	/*
	igraph_vector_init(tg, 8);
  	
  	VECTOR(v1)[2]=1; VECTOR(v1)[3]=2;
  	VECTOR(v1)[4]=2; VECTOR(v1)[5]=3;
  	VECTOR(v1)[6]=2; VECTOR(v1)[7]=2;
	  */
	igraph_vector_t edges;
	igraph_vector_init(&edges, 0);
	//VECTOR(edges)[0]=0; VECTOR(edges)[1]=1;
  	igraph_create(graph, &edges, 0, true);

	// Add graph attributes
	SETGAS(graph,"autname", tg->getAUTName().c_str());
	SETGAS(graph,"target_makespan", tg->getMakespanCalculator().c_str());

	// Add vertices
	int ret = igraph_add_vertices(graph, tg->getTasks().size(), 0);
	if(ret == IGRAPH_EINVAL) throw CastException("Could not add vertices to igraph.");

	for(set<Task>::const_iterator i = tg->getTasks().begin(); i != tg->getTasks().end(); i++)
	{
		Task task = *i;
		
		SETVAS(graph, "taskid", task.getId() - 1, task.getTaskId().c_str());
		SETVAS(graph, "taskname", task.getId() - 1, task.getName().c_str());
		SETVAN(graph, "workload", task.getId() - 1, task.getWorkload());
		SETVAN(graph, "max_width", task.getId() - 1, task.getMaxWidth());

		if (arch == NULL)
		{
			SETVAS(graph, "efficiency", task.getId() - 1, task.getEfficiencyString().c_str());
		}
		else
		{
			stringstream ss;
			
			for(int j = 1; j <= arch->getCoreNumber(); j++)
			{
				ss << task.getEfficiency(j) << " ";
			}

			SETVAS(graph, "efficiency", task.getId() - 1, ss.str().c_str());
		}
	}

	// Dump data to stream os
	int p[2];
	auto ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[1], "w"); 
	FILE *instream = fdopen (p[0], "r");

	// Spawn a thread that reads from the pipe as fast as it can.
	// This is required if igraph indend to output massive amounts of data
	auto t = std::thread([](FILE* instream, ostream* o){
		char c;
		while ((c = fgetc (instream)) != EOF)
		{
			(*o) << c;
		}
		fclose (instream);
	}, instream,&os);

	igraph_write_graph_graphml(graph,fake_fileptr,true); 
	fclose(fake_fileptr);

	t.join();
	close(p[0]);
	close(p[1]);

	igraph_destroy(graph);
	delete graph;
}

void
GraphML::dump(ostream& os, const StreamingAppData *data) const
{
	dump(os, data, NULL);
}

void
GraphML::dump(ostream& os, const StreamingAppData &data) const
{
	dump(os, &data, NULL);
}

void
GraphML::dump(ostream& os, const StreamingAppData &data, const Architecture &arch) const
{
	dump(os, &data, &arch);
}

Taskgraph*
GraphML::parse(istream &is) const
{
	Taskgraph *tg = new Taskgraph();

	// Open the file and get an igraph record
	// Initialize igraph and build a new object
	igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
	igraph_t *the_graph = new igraph_t();

	// Create a FILE* from istream by building a posix pipe. This wont work in windows...
	int p[2];
	auto ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[0], "r");   

	auto t = std::thread([](int file_descr, istream* data)
	{
		char c = data->get();
		while(!data->eof())
		{
			auto ans = write(file_descr, &c, 1);
			
			if(ans < 0)
			{
				throw runtime_error("Error: Pipe has been possessed X-(");
			}
			
			c = data->get();
		}
		
		close(file_descr);
	}, p[1], &is);

	// Parse input file
	igraph_read_graph_graphml(the_graph,fake_fileptr,0);

	// Clone the file and wait for the pipe to finish
	close(p[0]);
	t.join();

	set<Task> tasks;
	for(int id = 0; id < igraph_vcount(the_graph); id++)
	{
		Task task(id + 1, strcmp(VAS(the_graph, "taskid", id),"") != 0 ? VAS(the_graph, "taskid", id) : "UNNAMED_TASKID");
		task.setName(strcmp(VAS(the_graph, "taskname", id),"") != 0 ? VAS(the_graph, "taskname", id) : "UNNAMED_TASKNAME");
		task.setWorkload(!isnan((float)VAN(the_graph, "workload", id)) ? VAN(the_graph, "workload", id): 1.0);
		task.setMaxWidth((int)VAN(the_graph, "max_width", id) != INT_MIN ?  VAN(the_graph, "max_width", id) : 1);

		if (strcmp(VAS(the_graph, "efficiency", id), "") != 0)
		{
			task.setEfficiencyString(VAS(the_graph, "efficiency", id));
		}
		else
		{
			stringstream ss;
			ss << task.getMaxWidth();
			task.setEfficiencyString(string("fml:p <= ") + ss.str() + "? 1 : 1e-6");
		}

		tasks.insert(task);
	}

	tg->setTasks(tasks);

	tg->setAUTName(GAS(the_graph, "autname"));
	tg->setMakespanCalculator(GAS(the_graph, "target_makespan"));

	// Cleanup
	igraph_destroy(the_graph);
	delete the_graph;

	// TODO: parse links	
	return tg;
}

GraphML*
GraphML::clone() const
{
	return new GraphML();
}
