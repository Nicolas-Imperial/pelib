#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Taskgraph.hpp"
#include "Record.hpp"

#include <iostream>

namespace pelib{




  class Schedule // : public output..
  {
  public:
    void dump(std::ostream& os,const TaskgraphRecord& tgr,const Record& amploutput);

  };




}

#endif
