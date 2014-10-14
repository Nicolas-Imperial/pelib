#include "Taskgraph.hpp"

#include <GraphMLParser.hpp>
#include <GraphMLOutput.hpp>
#include <Platform.hpp>

#ifndef PELIB_GRAPHML_H
#define PELIB_GRAPHML_H

namespace pelib
{
	using namespace std;
	
	class GraphML : public GraphMLParser, public GraphMLOutput
	{
		public:
			virtual GraphML* clone() const;
			virtual Taskgraph* parse(std::istream &is) const;
			virtual void dump(std::ostream &str, const StreamingAppData *data) const;
			virtual void dump(std::ostream &str, const StreamingAppData &data) const;
			virtual	void dump(std::ostream &os, const StreamingAppData &data, const Platform &arch) const;
			virtual void dump(ostream& os, const StreamingAppData *data, const Platform *arch) const;
		protected:
	};
}

#endif
