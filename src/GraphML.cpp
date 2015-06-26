#include <libxml++/libxml++.h>
#include <exception>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

#include <signal.h>
#include <pthread.h>
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
#include <Link.hpp>

#include <CastException.hpp>
#include <ParseException.hpp>

using namespace pelib;
using namespace std;
using namespace xmlpp;

struct reader_args
{
	FILE *instream;
	ostream *o;
};
typedef struct reader_args reader_args_t;

static void*
thread_reader(void* aux)
{
	reader_args_t *args = (reader_args_t*)aux;
	
	char c;
	while ((c = fgetc (args->instream)) != EOF)
	{
		(*args->o) << c;
	}

	fclose(args->instream);
	return NULL;
}

struct writer_args
{
	int file_descr;
	istream *i;
};
typedef struct writer_args writer_args_t;

static void*
thread_writer(void* aux)
{
	writer_args_t *args = (writer_args_t*)aux;

	char c = args->i->get();
	while(!args->i->eof())
	{
		size_t ans = write(args->file_descr, &c, 1);
		
		if(ans < 0)
		{
			throw runtime_error("Error: Pipe has been possessed X-(");
		}
		
		c = args->i->get();
	}
	
	close(args->file_descr);
	return NULL;
}

GraphML::~GraphML()
{
	// Do nothing
}

void
GraphML::dump(ostream& os, const Record *data, const Platform *arch) const
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
	SETGAS(graph,"name", tg->getName().c_str());
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
		stringstream max_width;

		// If no platform is provided, just dump efficiency and max width as is.
		// Otherwise, compute and output each possible value one by one, and
		// replace any larger value of max_width than number of core, by number of cores 
		if (arch == NULL)
		{
			SETVAS(graph, "efficiency", task.getId() - 1, task.getEfficiencyString().c_str());
			max_width << task.getMaxWidth();
		}
		else
		{
			stringstream ss;
			
			for(int j = 1; j <= arch->getCoreNumber(); j++)
			{
				ss << task.getEfficiency(j) << " ";
			}

			SETVAS(graph, "efficiency", task.getId() - 1, ss.str().c_str());

			if(task.getMaxWidth() >= arch->getCoreNumber())
			{
				max_width << arch->getCoreNumber();
			}
		}

		SETVAS(graph, "max_width", task.getId() - 1, max_width.str().c_str());
	}

	// Dump data to stream os
	int p[2];
	int ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[1], "w"); 
	FILE *instream = fdopen (p[0], "r");

	// Spawn a thread that reads from the pipe as fast as it can.
	// This is required if igraph indend to output massive amounts of data
	pthread_attr_t attr;
	pthread_t thread;
	reader_args_t args;
	args.instream = instream;
	args.o = &os;

	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, &thread_reader, (void*) &args);

	igraph_write_graph_graphml(graph, fake_fileptr, true); 
	fclose(fake_fileptr);

	pthread_join(thread, NULL);
	//fclose (instream);
	close(p[0]);

	igraph_destroy(graph);
	delete graph;
}

void
GraphML::dump(ostream& os, const Record *data) const
{
	dump(os, data, NULL);
}

void
GraphML::dump(ostream& os, const Record &data) const
{
	dump(os, &data, NULL);
}

void
GraphML::dump(ostream& os, const Record &data, const Platform &arch) const
{
	dump(os, &data, &arch);
}

Taskgraph*
GraphML::parse(istream &is) const
{
	// Open the file and get an igraph record
	// Initialize igraph and build a new object
	igraph_i_set_attribute_table(&igraph_cattribute_table); //do this to enable attribute fetching
	igraph_t *the_graph = new igraph_t();

	// Create a FILE* from istream by building a posix pipe. This wont work in windows...
	int p[2];
	int ans = pipe(p);
	if(ans)
	{
		throw runtime_error("Error: no pipe :/");
	}

	FILE *fake_fileptr = fdopen(p[0], "r");   

	pthread_attr_t attr;
	pthread_t thread;
	writer_args_t args;
	args.file_descr = p[1];
	args.i = &is;

	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, &thread_writer, (void*) &args);

//	char ch;
//	while( ( ch = fgetc(fake_fileptr) ) != EOF )
//      		printf("%c",ch);
	// Parse input file
	igraph_read_graph_graphml(the_graph,fake_fileptr,0);

	// Clone the file and wait for the pipe to finish
	close(p[0]);
	pthread_join(thread, NULL);

	set<Task> tasks;
	for(int id = 0; id < igraph_vcount(the_graph); id++)
	{
		Task task(id + 1, strcmp(VAS(the_graph, "taskid", id), "") != 0 ? VAS(the_graph, "taskid", id) : "UNNAMED_TASKID");
		task.setName(strcmp(VAS(the_graph, "taskname", id),"") != 0 ? VAS(the_graph, "taskname", id) : "UNNAMED_TASKNAME");
		task.setWorkload(!isnan((float)VAN(the_graph, "workload", id)) ? VAN(the_graph, "workload", id): 1.0);
		const char *str = VAS(the_graph, "max_width", id);
		string max_width_str(str);
		boost::algorithm::to_lower(max_width_str);
		boost::algorithm::trim(max_width_str);
		float max_width;

		if(max_width_str.compare("inf") == 0 || max_width_str.compare("+inf") == 0)
		{
			max_width = std::numeric_limits<float>::infinity();
		}
		else
		{
			if(max_width_str.compare("-inf") == 0)
			{
				max_width = 1;
			}
			else
			{
				std::istringstream converter(max_width_str);
				converter >> max_width;

				if(converter.fail())
				{
					throw ParseException(std::string("Couln't convert literal \"").append(max_width_str).append("\" into type \"").append(typeid(max_width).name()).append("\"."));
				}
			}
		}
		task.setMaxWidth(max_width);
		//task.setMaxWidth((int)VAN(the_graph, "max_width", id) != INT_MIN ? VAN(the_graph, "max_width", id) : 1);

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

	// Add edges between tasks
	set<Link> links;
	for(int i = 0; i < igraph_ecount(the_graph); i++)
	{
		//printf("[%s:%s:%d] Edge number %d.\n", __FILE__, __FUNCTION__, __LINE__, i);
		int producer_id, consumer_id;

		igraph_edge(the_graph, i, &producer_id, &consumer_id);
		Task producer(producer_id + 1);
		Task consumer(consumer_id + 1);

		Link link(*tasks.find(producer), *tasks.find(consumer));
		links.insert(link);


		const Link &link_ref = *links.find(link);

		// Add the link as producer and consumer links of both endpoint tasks
		Task &producer_ref = (Task&)*tasks.find(producer);
		producer_ref.getConsumers().insert(&link_ref);
		Task &consumer_ref = (Task&)*tasks.find(consumer);
		consumer_ref.getProducers().insert(&link_ref);

		/*
		trace(producer_ref.getId());
		trace(consumer_ref.getId());
		trace(producer_ref.getConsumers().size());
		trace(consumer_ref.getConsumers().size());
		trace(producer_ref.getProducers().size());
		trace(consumer_ref.getProducers().size());
		*/
	}

	// Finally build the taskgraph
	Taskgraph *tg = new Taskgraph(tasks, links);
	tg->setName(GAS(the_graph, "name"));
	tg->setMakespanCalculator(GAS(the_graph, "target_makespan"));

	// Cleanup
	igraph_destroy(the_graph);
	delete the_graph;

	return tg;
}

GraphML*
GraphML::clone() const
{
	return new GraphML();
}
