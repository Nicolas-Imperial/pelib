#include <Taskgraph.hpp>

namespace pelib
{
	Taskgraph::Taskgraph()
	{
	}

	Taskgraph::Taskgraph(const Taskgraph* graph)
	{
		igraph_copy(&this->igraph, graph->getIGraph());
	}

	Taskgraph::~Taskgraph()
	{
		igraph_destroy(&this->igraph);
	}

	Taskgraph*
	Taskgraph::clone() const
	{
		return new Taskgraph();
	}

	const igraph_t*
	Taskgraph::getIGraph() const
	{
		return &igraph;
	}
}
