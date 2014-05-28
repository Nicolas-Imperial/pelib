#include <StreamingAppData.hpp>
#include <Algebra.hpp>

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

			virtual Algebra* buildAlgebra() const;
				
		protected:
			igraph_t igraph;
		private:		
	};
}

#endif
