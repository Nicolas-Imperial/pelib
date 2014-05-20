#include <libxml++/libxml++.h>
#include "Record.hpp"

extern "C"{
#include <igraph.h>
}

#ifndef PELIB_SCHEDULERECORD
#define PELIB_SCHEDULERECORD

namespace pelib
{
  class ScheduleRecord
  {
    friend class Schedule;
  public:
    ScheduleRecord() = delete;
    ScheduleRecord(xmlpp::DomParser* theSchedule);
    ScheduleRecord(const ScheduleRecord &rhs);

    ScheduleRecord& operator=(const ScheduleRecord &rhs);
    ~ScheduleRecord();
    
    
    Record toRecord() const;
    
  private:
    //TODO: Maybe store it in a more accessible format
    xmlpp::DomParser* theSchedule;
  };
}

#endif
