#include "Taskgraph.hpp"

#include <GraphMLParser.hpp>
#include <GraphMLOutput.hpp>
#include <Platform.hpp>

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
			virtual void dump(std::ostream &str, const Record *data) const;
			virtual void dump(std::ostream &str, const Record &data) const;
			virtual	void dump(std::ostream &os, const Record &data, const Platform &arch) const;
			virtual void dump(std::ostream &os, const Record *data, const Platform *arch) const;
		protected:
	};
}

#endif
