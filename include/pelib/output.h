#include <iostream>
#include <map>
#include <Record.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PELIB_OUTPUT
#define PELIB_OUTPUT

// /!\ the content of argv is freed after this function is run
void pelib_dump(std::ostream& cout, std::map<const char*, pelib::Record*> records, size_t argc, char** argv);

#endif

#ifdef __cplusplus
}
#endif
