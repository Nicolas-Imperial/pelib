#include "Algebra.hpp"

#include <vector>

#ifndef PELIB_TASKGRAPHRECORD
#define PELIB_TASKGRAPHRECORD

extern "C"{
#include <igraph.h>
}

namespace pelib
{
  struct Vertex_info;
  class TaskgraphRecord
  {
  public:
    TaskgraphRecord() = delete;
    TaskgraphRecord(igraph_t *graph);
    TaskgraphRecord(const TaskgraphRecord &rhs);
    TaskgraphRecord(const TaskgraphRecord& tgr, const Algebra& record);
     TaskgraphRecord& operator=(const TaskgraphRecord &rhs);
    ~TaskgraphRecord();
    
    explicit TaskgraphRecord(const Algebra& record);
    
    // The architecture information is used at toRecord.
    // If no architecture has been set, default
    // values are used.
    void setArchitecture(const Algebra& architecture);
//    Record toRecord() const;
    Algebra toAlgebra(const Algebra& architecture) const;
    
    std::vector<std::string> get_taskids() const;
//    float getTargetMakespan() const;
    float getTargetMakespan(const Algebra& architecture) const;
    const char* get_autname() const;

    // That should be private
    igraph_t *graph;
  private:
    void merge_taskgraph_record(const Algebra& record);
    std::vector<Vertex_info> buildVertexVector(void) const;
//    float makespan_random(const std::vector<Vertex_info>& tasks );
//    Record* architecture; // = nullptr;
  };
}

#endif
