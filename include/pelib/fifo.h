/*
 * fifo.h
 *
 *  Created on: 26 Jan 2012
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

#include <stddef.h>

#include <pelib/template.h>

#ifndef FIFO_H
#define FIFO_H

enum pelib_fifo_operation
{
	PELIB_CFIFO_PUSH,
	PELIB_CFIFO_POP
};
typedef enum pelib_fifo_operation pelib_fifo_operation_t;
#endif

#if PELIB_CONCAT_2(DONE_cfifo_, CFIFO_T) == 0

#define cfifo(elem) PELIB_CONCAT_2(cfifo_, elem)
#define cfifo_t(elem) PELIB_CONCAT_2(cfifo(elem), _t)

#define pelib_cfifo_push(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _push)
#define pelib_cfifo_fill(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _fill)
#define pelib_cfifo_pop(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _pop)
#define pelib_cfifo_discard(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _discard)
#define pelib_cfifo_peek(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _peek)
#define pelib_cfifo_last(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _last)
#define pelib_cfifo_is_full(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _is_full)
#define pelib_cfifo_is_empty(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _is_empty)
#define pelib_cfifo_string_content(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _string_content)
#define pelib_cfifo_printf_content(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _printf_content)
#define pelib_cfifo_pushmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _pushmem)
#define pelib_cfifo_popmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _popmem)
#define pelib_cfifo_peekmem(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _peekmem)
#define pelib_cfifo_popfifo(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _popfifo)
#define pelib_cfifo_length(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _length)
#define pelib_cfifo_last(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _last)
#define pelib_cfifo_capacity(elem) PELIB_CONCAT_3(pelib_, cfifo(elem), _capacity)

struct cfifo(CFIFO_T)
{
	size_t read;
	size_t write;
	//int id;
	pelib_fifo_operation_t last_op;
	size_t capacity;
	volatile CFIFO_T *buffer;
};
typedef struct cfifo(CFIFO_T) cfifo_t(CFIFO_T);

#define STRUCT_T cfifo_t(CFIFO_T)
#include <pelib/structure.h>

#define PELIB_FIFO_ELEM_MAX_CHAR        11

int
pelib_cfifo_push(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T);
size_t
pelib_cfifo_fill(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t);
CFIFO_T
pelib_cfifo_pop(CFIFO_T)(cfifo_t(CFIFO_T)*);
size_t
pelib_cfifo_discard(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t);
CFIFO_T
pelib_cfifo_peek(CFIFO_T)(cfifo_t(CFIFO_T)*, size_t offset);
int
pelib_cfifo_is_full(CFIFO_T)(cfifo_t(CFIFO_T)*);
int
pelib_cfifo_is_empty(CFIFO_T)(cfifo_t(CFIFO_T)*);
size_t
pelib_cfifo_pushmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T*, size_t);
size_t
pelib_cfifo_popmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T*, size_t);
size_t
pelib_cfifo_peekmem(CFIFO_T)(cfifo_t(CFIFO_T)*, CFIFO_T*, size_t, size_t);
size_t
pelib_cfifo_popfifo(CFIFO_T)(cfifo_t(CFIFO_T)*, cfifo_t(CFIFO_T)*, size_t);
size_t
pelib_cfifo_length(CFIFO_T)(cfifo_t(CFIFO_T));
CFIFO_T
pelib_cfifo_last(CFIFO_T)(cfifo_t(CFIFO_T)*);
size_t
pelib_cfifo_capacity(CFIFO_T)(cfifo_t(CFIFO_T)*);
#endif
#undef CFIFO_T
