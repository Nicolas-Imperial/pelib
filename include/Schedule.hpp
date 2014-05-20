
#include "IGraph.hpp"
#include "Record.hpp"
#include "ScheduleRecord.hpp"

#include <iostream>

#ifndef SCHEDULE_H
#define SCHEDULE_H

namespace pelib{
  //class ScheduleRecord;
  class Schedule // : public output..
  {
  public:
    void dump(std::ostream& os,const TaskgraphRecord& tgr,const Record& amploutput);
    void dump(std::ostream& os,const ScheduleRecord& sr);
    ScheduleRecord parse(std::istream &is);
  };
}

#endif
