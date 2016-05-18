#include <pelib/integer.h>
#include <pelib/string.h>

#ifndef STRING_INTEGER_H
#define STRING_INTEGER_H

#define PAIR_KEY_T string
#define PAIR_VALUE_T int
#include <pelib/pair.h>
#define DONE_pair_string_int 1 

#define ITERATOR_T pair_t(string, int)
#include <pelib/iterator.h>
#define DONE_iterator_pair_string_int 1 

#define MAP_KEY_T string
#define MAP_VALUE_T int
#include <pelib/map.h>
#define DONE_map_string_int 1

#endif

