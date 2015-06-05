#include <PlatformParser.hpp>
#include <PlatformOutput.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PLATFORM_FACTORY
#define PLATFORM_FACTORY

pelib::PlatformParser* create_parser();
void delete_parser(pelib::PlatformParser*);

pelib::PlatformOutput* create_output();
void delete_output(pelib::PlatformOutput*);

#endif

#ifdef __cplusplus
}
#endif
