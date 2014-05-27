#include <Taskgraph.hpp>

namespace pelib
{
/*
	Taskgraph::Taskgraph(const std::string name, const igraph_t *tg) : Data(name)
	{
		igraph_copy(&igraph, tg);
	}
*/
	Taskgraph::Taskgraph(const std::string name) : StreamingAppData(name) { /* Do nothing */ };

	Taskgraph::Taskgraph(const Taskgraph* graph) : StreamingAppData(graph->getName())
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
		return new Taskgraph(name);
	}

	const igraph_t*
	Taskgraph::getIGraph() const
	{
		return &igraph;
	}
}
