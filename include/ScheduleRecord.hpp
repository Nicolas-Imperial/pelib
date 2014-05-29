#include <libxml++/libxml++.h>

extern "C"{
#include <igraph.h>
}

#include "Algebra.hpp"

#ifndef PELIB_SCHEDULERECORD
#define PELIB_SCHEDULERECORD

namespace pelib
{
  class ScheduleRecord
  {
  public:
    ScheduleRecord() = delete;
    ScheduleRecord(xmlpp::DomParser* theSchedule);
    ScheduleRecord(const ScheduleRecord &rhs);

    ScheduleRecord& operator=(const ScheduleRecord &rhs);
    ~ScheduleRecord();
    
    
    Algebra toAlgebra() const;
    
  private:
    //TODO: Maybe store it in a more accessible format
    xmlpp::DomParser* theSchedule;
  };
}

#endif
