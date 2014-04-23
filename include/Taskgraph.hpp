#include "RecordParser.hpp"
#include "RecordOutput.hpp"
#ifndef PELIB_TASKGRAPH
#define PELIB_TASKGRAPH
namespace pelib
{
  class Taskgraph: public RecordParser, public RecordOutput
  {
  public:
    void dump(std::ostream&, const pelib::Record&) const{} //stub
    void dump(std::ostream&, const pelib::Data*) const {} //stub

    Record parse(std::istream&);
    Record parse(std::istream& data, size_t processors);
  };
}






#endif
