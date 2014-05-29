#include <Taskgraph.hpp>

namespace pelib
{
	Taskgraph::Taskgraph()
	{
	}

	Taskgraph::Taskgraph(const Taskgraph* graph)
	{
	}

	Taskgraph::~Taskgraph()
	{
	}

	Taskgraph*
	Taskgraph::clone() const
	{
		return new Taskgraph();
	}

	const igraph_t*
	Taskgraph::getIGraph() const
	{
		return NULL;
	}

	Algebra
	Taskgraph::buildAlgebra() const
	{
		return Algebra();
	}
}
