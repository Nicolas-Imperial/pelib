#include <iostream>
#include <Record.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PELIB_PARSER
#define PELIB_PARSER

// /!\ the content of argv is freed after this function is run
pelib::Record* pelib_parse(std::istream& cin, size_t argc, char** argv);
void pelib_delete(pelib::Record*);

#endif

#ifdef __cplusplus
}
#endif
