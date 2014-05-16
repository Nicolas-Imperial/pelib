#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH

#include <vector>
//#include <istream>
#include <iostream>
namespace pelib
{
class IGraph;
}
#include "TaskgraphRecord.hpp"


namespace pelib
{
  class IGraph
  {
  public:
    void dump(std::ostream&, const pelib::TaskgraphRecord&) const;
    TaskgraphRecord parse(std::istream&);

    void duplicate_tasks(TaskgraphRecord& data, const std::vector<int> & to_duplicate);
  };
}






#endif
