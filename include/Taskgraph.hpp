#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/regex.hpp>
#include <iomanip>

#include <StreamingAppData.hpp>

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

namespace pelib
{
	class Taskgraph: public StreamingAppData
	{
		public:
			Taskgraph();
			Taskgraph(const Taskgraph* graph);
			virtual ~Taskgraph();

			virtual
			Taskgraph*
			clone() const;

			virtual
			const igraph_t*
			getIGraph() const;
				
		protected:
			igraph_t igraph;
		private:		
	};
}

#endif
