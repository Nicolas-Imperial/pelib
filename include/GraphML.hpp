#include <vector>
#include <iostream>

#include "TaskgraphRecord.hpp"

#ifndef PELIB_GRAPHML
#define PELIB_GRAPHML

namespace pelib
{
  class GraphML
  {
  public:
    void dump(std::ostream&, const pelib::TaskgraphRecord&) const;
    TaskgraphRecord parse(std::istream&);

    void duplicate_tasks(TaskgraphRecord& data, const std::vector<int> & to_duplicate);
  };
}






#endif
