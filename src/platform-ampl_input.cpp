#include <pelib/platform_factory.hpp>
#include <AmplPlatform.hpp>

using namespace std;
using namespace pelib;

#ifdef __cplusplus
extern "C" {
#endif

PlatformParser*
create_parser()
{
	return new AmplPlatform();
}

void
delete_parser(PlatformParser *parser)
{
	delete parser;
}

PlatformOutput* create_output()
{
	return new AmplPlatform();
}

void
delete_output(PlatformOutput *output)
{
	delete output;
}

#ifdef __cplusplus
}
#endif

