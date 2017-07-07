/*
 * pair.h
 *
 *  Created on: 5 Sep 2011
 *  Copyright 2011 Nicolas Melot
 *
 * This file is part of pelib.
 * 
 *     pelib is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     pelib is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <pelib/template.h>
#if PELIB_CONCAT_4(DONE_pair_, PAIR_KEY_T, _, PAIR_VALUE_T) == 0

#define pair(key, value) PELIB_CONCAT_4(pair_, key, _, value)
#define pair_t(key, value) PELIB_CONCAT_2(pair(key, value), _t)

/** Generic static pair **/
struct pair(PAIR_KEY_T, PAIR_VALUE_T)
{
	PAIR_KEY_T key;
	PAIR_VALUE_T value;
};
typedef struct pair(PAIR_KEY_T, PAIR_VALUE_T) pair_t(PAIR_KEY_T, PAIR_VALUE_T);

#define STRUCT_T pair_t(PAIR_KEY_T, PAIR_VALUE_T)
#include <pelib/structure.h>

#endif /* PAIR_H_ */

// Make sure the symbol does not propagate any further
#undef PAIR_KEY_T
#undef PAIR_VALUE_T
#ifdef __cplusplus
}
#endif
