#ifndef PELIB_TASKGRAPHRECORD
#define PELIB_TASKGRAPHRECORD


namespace pelib
{
  class TaskgraphRecord;
}
#include "Taskgraph.hpp"
#include "Record.hpp"

extern "C"{
#include <igraph.h>
}

namespace pelib
{
  class TaskgraphRecord
  {
    friend class Taskgraph;
  public:
    TaskgraphRecord() = delete;
    TaskgraphRecord(igraph_t *graph);
    TaskgraphRecord(const TaskgraphRecord &rhs);
     TaskgraphRecord& operator=(const TaskgraphRecord &rhs);
    ~TaskgraphRecord();
    
    explicit TaskgraphRecord(const Record& record);
    
    Record toRecord() const;
    
    
  private:
    igraph_t *graph;
  };
}

#endif
