#include <pelib/string_integer.h>

// Now include the generic set implementation
#define PAIR_KEY_T string
#define PAIR_VALUE_T int
#include "pelib/pair.c"

#define ITERATOR_T pair_t(string, int)
#include "pelib/iterator.c"

// Now include the generic set implementation
#define MAP_KEY_T string
#define MAP_VALUE_T int
#include "pelib/map.c"

