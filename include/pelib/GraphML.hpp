#include <pelib/Taskgraph.hpp>

#include <pelib/GraphMLParser.hpp>
#include <pelib/GraphMLOutput.hpp>
#include <pelib/Platform.hpp>

#ifndef PELIB_GRAPHML_H
#define PELIB_GRAPHML_H

namespace pelib
{
	class GraphML : public GraphMLParser, public GraphMLOutput
	{
		public:
			virtual ~GraphML();
			virtual GraphML* clone() const;
			virtual Taskgraph* parse(std::istream &is) const;
			virtual void dump(std::ostream &str, const Taskgraph *data) const;
			virtual void dump(std::ostream &str, const Taskgraph &data) const;
			virtual	void dump(std::ostream &os, const Taskgraph &data, const Platform &arch) const;
			virtual void dump(std::ostream &os, const Taskgraph *data, const Platform *arch) const;
		protected:
	};
}

#endif