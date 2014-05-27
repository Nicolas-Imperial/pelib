#include <AmplInput.hpp>

#include <Architecture.hpp>
#include <AmplArchitecture.hpp>

using namespace pelib;
using namespace std;

void
AmplArchitecture::dump(ostream& os, const Architecture *sched) const
{
	//sr.theSchedule->get_document()->write_to_stream(os);
}

Architecture*
AmplArchitecture::parse(istream &is)
{
	AmplInput reader;
	Algebra alg_arch = reader.parse(is);

	Architecture arch;
/*
  using namespace xmlpp;
  DomParser* parser = new DomParser();
  parser->parse_stream(is);

  return ScheduleRecord(parser);
*/
	return new Architecture();
}

AmplArchitecture*
AmplArchitecture::clone() const
{
	return new AmplArchitecture();
}
