/*
 * iterator.h
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

#include <pelib/template.h>
#if PELIB_CONCAT_2(DONE_iterator_, ITERATOR_T) == 0

#define iterator(elem) PELIB_CONCAT_2(iterator_, elem)
#define iterator_t(elem) PELIB_CONCAT_2(iterator(elem), _t)

#define pelib_iterator_next(value) PELIB_CONCAT_3(pelib_, iterator(value), _next)
#define pelib_iterator_read(value) PELIB_CONCAT_3(pelib_, iterator(value), _read)

/** Generic static iterator **/
struct iterator(ITERATOR_T)
{
	struct iterator(ITERATOR_T) *previous, *next;
	ITERATOR_T value;
};
typedef struct iterator(ITERATOR_T) iterator_t(ITERATOR_T);

iterator_t(ITERATOR_T)*
pelib_iterator_next(ITERATOR_T)(iterator_t(ITERATOR_T)*);

ITERATOR_T
pelib_iterator_read(ITERATOR_T)(iterator_t(ITERATOR_T)*);

#define STRUCT_T iterator_t(ITERATOR_T)
#include <pelib/structure.h>

#endif /* ITERATOR_H_ */

// Make sure the symbol does not propagate any further
#undef ITERATOR_T
