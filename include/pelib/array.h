/*
 * array.h
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
#if PELIB_CONCAT_2(DONE_array_, ARRAY_T) == 0

#define array(elem) PELIB_CONCAT_2(array_, elem)
#define array_t(elem) PELIB_CONCAT_2(array(elem), _t)

#define pelib_array_loadfilename(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilename)
#define pelib_array_loadfilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilenamebinary)
#define pelib_array_preloadfilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _preloadfilenamebinary)
#define pelib_array_loadfilenamewindowbinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _loadfilenamewindowbinary)
#define pelib_array_storefilename(elem) PELIB_CONCAT_3(pelib_, array(elem), _storefilename)
#define pelib_array_storefilenamebinary(elem) PELIB_CONCAT_3(pelib_, array(elem), _storefilenamebinary)
#define pelib_array_checkascending(elem) PELIB_CONCAT_3(pelib_, array(elem), _checkascending)
#define pelib_array_read(elem) PELIB_CONCAT_3(pelib_, array(elem), _read)
#define pelib_array_write(elem) PELIB_CONCAT_3(pelib_, array(elem), _write)
#define pelib_array_append(elem) PELIB_CONCAT_3(pelib_, array(elem), _append)
#define pelib_array_length(elem) PELIB_CONCAT_3(pelib_, array(elem), _length)
#define pelib_array_capacity(elem) PELIB_CONCAT_3(pelib_, array(elem), _capacity)
#define pelib_array_compare(elem) PELIB_CONCAT_3(pelib_, array(elem), _compare)

struct array(ARRAY_T)
  {
    size_t capacity;
    size_t length;
    ARRAY_T* data;
  };
typedef struct array(ARRAY_T) array_t(ARRAY_T);

#define STRUCT_T array_t(ARRAY_T)
#include <pelib/structure.h>

array_t(ARRAY_T)*
pelib_array_loadfilename(ARRAY_T)(char*);
array_t(ARRAY_T)* pelib_array_loadfilenamebinary(ARRAY_T)(char*);
array_t(ARRAY_T)* pelib_array_preloadfilenamebinary(ARRAY_T)(char*);
array_t(ARRAY_T)* pelib_array_loadfilenamewindowbinary(ARRAY_T)(char*, size_t, size_t);
//void
//pelib_array_printf(ARRAY_T)
//(array_t(ARRAY_T)*);
//array_t(ARRAY_T)* pelib_array_alloc(ARRAY_T)(int);
//void
//pelib_array_free(ARRAY_T)
//(array_t(ARRAY_T)*);
//void
//pelib_array_prefree(ARRAY_T)
//(array_t(ARRAY_T)*);
int
pelib_array_storefilename(ARRAY_T)
(array_t(ARRAY_T)*, char*);
int
pelib_array_storefilenamebinary(ARRAY_T)
(array_t(ARRAY_T)*, char*);
int
pelib_array_checkascending(ARRAY_T)
(array_t(ARRAY_T)*);
ARRAY_T
pelib_array_read(ARRAY_T)
(array_t(ARRAY_T)*, size_t);
int
pelib_array_write(ARRAY_T)
(array_t(ARRAY_T)*, size_t, ARRAY_T);
int
pelib_array_append(ARRAY_T)
(array_t(ARRAY_T)*, ARRAY_T);
size_t
pelib_array_length(ARRAY_T)
(array_t(ARRAY_T)*);
size_t
pelib_array_capacity(ARRAY_T)
(array_t(ARRAY_T)*);
int
pelib_array_compare(ARRAY_T)
(array_t(ARRAY_T)*, array_t(ARRAY_T)*);

#endif /* ARRAY_H_ */

// Make sure the symbol does not propagate any further
#undef ARRAY_T
