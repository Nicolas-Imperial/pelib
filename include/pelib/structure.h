/*
 * structure.h
 *
 *  Created on: 20 Feb 2012
 *  Copyright 2012 Nicolas Melot
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

#include <stdio.h>
#include <stddef.h>

#include <pelib/template.h>

// Prevents several code generation for the same data structure
#if PELIB_CONCAT_2(DONE_, STRUCT_T) == 0

#ifndef STRUCT_T
#error Using generic structure without a type
#endif

#define pelib_alloc_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_buffer)
#define pelib_set_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_set_buffer)
#define pelib_alloc_struct(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_struct)
#define pelib_alloc(elem) PELIB_CONCAT_3(pelib_,elem,_alloc)
#define pelib_alloc_collection(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_collection)
#define pelib_alloc_from(elem) PELIB_CONCAT_3(pelib_,elem,_alloc_from)
#define pelib_init(elem) PELIB_CONCAT_3(pelib_,elem,_init)
#define pelib_copy(elem) PELIB_CONCAT_3(pelib_,elem,_copy)
#define pelib_free_buffer(elem) PELIB_CONCAT_3(pelib_,elem,_free_buffer)
#define pelib_free_struct(elem) PELIB_CONCAT_3(pelib_,elem,_free_struct)
#define pelib_free(elem) PELIB_CONCAT_3(pelib_,elem,_free)
#define pelib_compare(elem) PELIB_CONCAT_3(pelib_,elem,_compare)
#define pelib_printf(elem) PELIB_CONCAT_3(pelib_,elem,_printf)
#define pelib_printf_detail(elem) PELIB_CONCAT_3(pelib_,elem,_printf_detail)
#define pelib_fwrite(elem) PELIB_CONCAT_3(pelib_,elem,_fwrite)
#define pelib_fread(elem) PELIB_CONCAT_3(pelib_,elem,_fread)
#define pelib_string(elem) PELIB_CONCAT_3(pelib_,elem,_string)
#define pelib_string_detail(elem) PELIB_CONCAT_3(pelib_,elem,_string_detail)

STRUCT_T *
pelib_alloc_struct(STRUCT_T)();
int
pelib_alloc_buffer(STRUCT_T)(STRUCT_T*, size_t);
int
pelib_set_buffer(STRUCT_T)(STRUCT_T*, void*, size_t, void*);
STRUCT_T *
pelib_alloc(STRUCT_T)();
STRUCT_T *
pelib_alloc_collection(STRUCT_T)(size_t);
STRUCT_T *
pelib_alloc_from(STRUCT_T)(void*, size_t, void*);
int
pelib_init(STRUCT_T)(STRUCT_T *);
int
pelib_copy(STRUCT_T)(STRUCT_T, STRUCT_T*);
int
pelib_free_struct(STRUCT_T)(STRUCT_T *);
int
pelib_free_buffer(STRUCT_T)(STRUCT_T *);
int
pelib_free(STRUCT_T)(STRUCT_T *);
int
pelib_compare(STRUCT_T)(STRUCT_T, STRUCT_T);
int
pelib_printf(STRUCT_T)(STRUCT_T);
int
pelib_printf_detail(STRUCT_T)(STRUCT_T, int);
size_t
pelib_fwrite(STRUCT_T)(STRUCT_T, size_t, size_t, FILE*);
size_t
pelib_fread(STRUCT_T)(STRUCT_T*, size_t, size_t, FILE*);
char*
pelib_string(STRUCT_T)(STRUCT_T);
char*
pelib_string_detail(STRUCT_T)(STRUCT_T, int);

#endif
#undef STRUCT_T
