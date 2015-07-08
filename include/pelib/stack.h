/*
 * stack.h
 *
 *  Created on: 18 Oct 2011
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

#include <pthread.h>

#include <pelib/template.h>

#ifndef STACK_T
#error Using generic stack without a type
#endif

#if PELIB_CONCAT_2(DONE_stack_, STACK_T) == 0

#define stackelem(elem) PELIB_CONCAT_2(stackelem_, elem)
#define stackelem_t(elem) PELIB_CONCAT_2(stackelem(elem), _t)

#define pelib_stackelem_write(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _write)
#define pelib_stackelem_read(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _read)
#define pelib_stackelem_check(elem) PELIB_CONCAT_3(pelib_, stackelem(elem), _check)

#define stack(elem) PELIB_CONCAT_2(stack_, elem)
#define stack_t(elem) PELIB_CONCAT_2(stack(elem), _t)

#define pelib_stack_check(elem) PELIB_CONCAT_3(pelib_, stack(elem), _check)
#define pelib_stack_isempty(elem) PELIB_CONCAT_3(pelib_, stack(elem), _isempty)
#define pelib_stack_push(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push)
#define pelib_stack_pop(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop)
#define pelib_stack_peek(elem) PELIB_CONCAT_3(pelib_, stack(elem), _peek)
#define pelib_stack_push_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_safe)
#define pelib_stack_pop_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_safe)
#define pelib_stack_push_elem(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_elem)
#define pelib_stack_pop_elem(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_elem)
#define pelib_stack_push_elem_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_elem_safe)
#define pelib_stack_pop_elem_safe(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_elem_safe)
#define pelib_stack_push_safe_managed(elem) PELIB_CONCAT_3(pelib_, stack(elem), _push_safe_managed)
#define pelib_stack_pop_safe_managed(elem) PELIB_CONCAT_3(pelib_, stack(elem), _pop_safe_managed)

struct
stackelem(STACK_T)
  {
    STACK_T buffer;
    struct stackelem(STACK_T) *next;
  };
typedef struct stackelem(STACK_T) stackelem_t(STACK_T);

#define STRUCT_T stackelem_t(STACK_T)
#include <pelib/structure.h>

int
pelib_stackelem_write(STACK_T)(stackelem_t(STACK_T)*, STACK_T);

int
pelib_stackelem_read(STACK_T)(stackelem_t(STACK_T)*, STACK_T*);

int
pelib_stackelem_check(STACK_T)(stackelem_t(STACK_T)*);

struct stack(STACK_T)
  {
    stackelem_t(STACK_T) * top;
    pthread_mutex_t lock;
  };
typedef struct stack(STACK_T) stack_t(STACK_T);

#define STRUCT_T stack_t(STACK_T)
#include <pelib/structure.h>

int
pelib_stack_check(STACK_T)(stack_t(STACK_T) *);

int
pelib_stack_isempty(STACK_T)(stack_t(STACK_T) *);

int
pelib_stack_push(STACK_T)(stack_t(STACK_T) *, STACK_T);

int
pelib_stack_pop(STACK_T)(stack_t(STACK_T) *, STACK_T*);

int
pelib_stack_peek(STACK_T)(stack_t(STACK_T) *, STACK_T*);

int
pelib_stack_push_safe(STACK_T)(stack_t(STACK_T) *, STACK_T);

int
pelib_stack_pop_safe(STACK_T)(stack_t(STACK_T) *, STACK_T*);

int
pelib_stack_push_elem(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)*);

int
pelib_stack_pop_elem(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)**);

int
pelib_stack_push_elem_safe(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T) *);

int
pelib_stack_pop_elem_safe(STACK_T)(stack_t(STACK_T) *, stackelem_t(STACK_T)**);

int
pelib_stack_push_safe_managed(STACK_T)(stack_t(STACK_T) *, stack_t(STACK_T) *, STACK_T);

int
pelib_stack_pop_safe_managed(STACK_T)(stack_t(STACK_T) *, stack_t(STACK_T) *, STACK_T*);

#endif

// Make sure the symbol does not propagate any further
#undef STACK_T
