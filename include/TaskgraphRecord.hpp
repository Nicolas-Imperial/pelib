#ifndef PELIB_TASKGRAPHRECORD
#define PELIB_TASKGRAPHRECORD


namespace pelib
{
  class TaskgraphRecord;
}
#include "Taskgraph.hpp"
#include "Record.hpp"
#include <vector>

extern "C"{
#include <igraph.h>
}

namespace pelib
{
  struct Vertex_info;
  class TaskgraphRecord
  {
    friend class Taskgraph;
  public:
    TaskgraphRecord() = delete;
    TaskgraphRecord(igraph_t *graph);
    TaskgraphRecord(const TaskgraphRecord &rhs);
    TaskgraphRecord(const TaskgraphRecord& tgr, const Record& record);
     TaskgraphRecord& operator=(const TaskgraphRecord &rhs);
    ~TaskgraphRecord();
    
    explicit TaskgraphRecord(const Record& record);
    
    // The architecture information is used at toRecord.
    // If no architecture has been set, default
    // values are used.
    void setArchitecture(const Record& architecture);
    Record toRecord() const;
    
    std::vector<std::string> get_taskids() const;
    float get_target_makespan() const;
    const char* get_autname() const;
  private:
    std::vector<Vertex_info> buildVertexVector(void) const;
    igraph_t *graph;
    Record* architecture = nullptr;
  };
}

#endif
