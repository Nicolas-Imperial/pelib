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
 *     but WITHOUT ANY WARRANTY without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with pelib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*
static uint64_t
get_mask()
{
  return ((uint64_t) - 1 & (((uint64_t) 1 << (sizeof(uint64_t) * 8 / 2)) - 1));
}

static uint64_t
add(uint64_t a, int value)
{
  return (((a >> (sizeof(uint64_t) * 8 / 2)) + value) << (sizeof(uint64_t) * 8
      / 2)) | get_mask();
}
*/

int
pelib_init(stackelem_t(STACK_T))(stackelem_t(STACK_T) *elem)
  {
    assert(elem != NULL);
    elem->next = NULL;

    return 0;
  }

int
pelib_stackelem_write(STACK_T)(stackelem_t(STACK_T) *elem, STACK_T buffer)
  {
    assert(pelib_stackelem_check(STACK_T)(elem) == 0);

    return pelib_copy(STACK_T)(buffer, &elem->buffer);
  }

int
pelib_stackelem_read(STACK_T)(stackelem_t(STACK_T) *elem, STACK_T* buffer)
  {
    assert(buffer != NULL);
    assert(pelib_stackelem_check(STACK_T)(elem) == 0);

    return pelib_copy(STACK_T)(elem->buffer, buffer);
  }

int
pelib_stackelem_check(STACK_T)(stackelem_t(STACK_T) *elem)
  {
    assert(elem != NULL);

    return 0;
  }

stack_t(STACK_T) *
pelib_alloc(stack_t(STACK_T))(void* aux)
  {
    stack_t(STACK_T) *res;

    res = malloc(sizeof(stack_t(STACK_T)));
    assert(res != NULL);

    if (res == NULL)
    return NULL;

    res->top = NULL;

    return res;
  }

int
pelib_init(stack_t(STACK_T))(stack_t(STACK_T) *s)
  {
    pthread_mutexattr_t attr;

    assert(s != NULL);

    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&s->lock, &attr);
    s->top = NULL;

    return 0;
  }

int
pelib_free(stack_t(STACK_T))(stack_t(STACK_T) *s)
  {
    pthread_mutex_destroy(&s->lock);
    free(s);

    return 0;
  }

int
pelib_stack_check(STACK_T)(stack_t(STACK_T) *s)
  {
    assert(s != NULL);

    return 0;
  }

int
pelib_stack_isempty(STACK_T)(stack_t(STACK_T) *s)
  {
    return s->top == NULL;
  }

int
pelib_stack_push(STACK_T)(stack_t(STACK_T) *s, STACK_T buffer)
  {
    stackelem_t(STACK_T) * elem;

    elem = malloc(sizeof(stackelem_t(STACK_T)));
    assert(elem != NULL);
    if (elem == NULL)
    return -1;

    pelib_init(stackelem_t(STACK_T))(elem);
    pelib_stackelem_write(STACK_T)(elem, buffer);

    pelib_stack_push_elem(STACK_T)(s, elem);

    return 0;
  }

int
pelib_stack_pop(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    stackelem_t(STACK_T) * elem;

    if (pelib_stack_peek(STACK_T)(s, buffer) != 0)
      {
        return -1;
      }

    pelib_stack_pop_elem(STACK_T)(s, &elem);
    free(elem);

    return 0;
  }

int
pelib_stack_peek(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    if (s->top == NULL)
      {
        return -1;
      }

    pelib_stackelem_read(STACK_T)(s->top, buffer);

    return 0;
  }

int
pelib_stack_push_safe(STACK_T)(stack_t(STACK_T) *s, STACK_T buffer)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_push(STACK_T)(s, buffer);
    pthread_mutex_unlock(&s->lock);

#if 0 // Non-blocking push
    stackelem_t(STACK_T) *elem;

    elem = pelib_alloc(stackelem_t(STACK_T))();
    assert(elem != NULL);
    if (elem == NULL)
    return -1;

    pelib_init(stackelem_t(STACK_T))(elem, stack->elem_size);
    pelib_stackelem_write(elem, buffer);

    stack_push_elem_safe(STACK_T)(stack, elem);
#endif

    return 0;
  }

int
pelib_stack_pop_safe(STACK_T)(stack_t(STACK_T) *s, STACK_T* buffer)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_pop(STACK_T)(s, buffer);
    pthread_mutex_unlock(&s->lock);

#if 0 // Non-blocking pop
    stackelem_t(STACK_T) *elem;

    if (stack_pop_elem_safe(STACK_T)(stack, &elem) != 0)
      {
        return -1;
      }
    stackelem_read(STACK_T)(elem, buffer);
    stackelem_free(stackelem_t(STACK_T))(elem);

#endif

    return 0;
  }

int
pelib_stack_push_elem(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) *elem)
  {
    assert(pelib_stack_check(STACK_T)(s) == 0);
    assert(s->top != elem);

    // Here comes the swap
    elem->next = s->top;
    s->top = elem;

    return 0;
  }

int
pelib_stack_pop_elem(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) **elem)
  {
    assert(pelib_stack_check(STACK_T)(s) == 0);

    if (s->top == NULL)
      {
        return -1;
      }

    // Here comes the swap
    *elem = s->top;
    s->top = (*elem)->next;

    (*elem)->next = NULL;

    return 0;
  }

int
pelib_stack_push_elem_safe(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) *elem)
  {
    assert(s->top != elem);

    pthread_mutex_lock(&s->lock);
    pelib_stack_push_elem(STACK_T)(s, elem);
    pthread_mutex_unlock(&s->lock);

#if 0 // Non-blocking
    stackelem_t(STACK_T) *old_top;

    assert(stack->elem_size == elem->size);
    assert(stack_check(STACK_T)(stack) == 0);
    assert(elem->next == NULL);

    // Here comes the swap
    elem->next = stack->top;
    old_top = elem->next;
    while (!cas(&stack->top, &old_top, &elem, &stack->lock))
      {
        elem->next = stack->top;
        old_top = elem->next;
      }

#endif

    return 0;
  }

int
pelib_stack_pop_elem_safe(STACK_T)(stack_t(STACK_T) *s, stackelem_t(STACK_T) **elem)
  {
    pthread_mutex_lock(&s->lock);
    pelib_stack_pop_elem(STACK_T)(s, elem);
    pthread_mutex_unlock(&s->lock);

#if 0
    stackelem_t(STACK_T) *old_top;

    assert(stack_check(s) == 0);
    assert(stack->top != NULL);

    // Here comes the swap
#if defined FIXED_ABA && 0
    uint64_t oldcounter, newcounter;
    do
      {
        oldcounter = s->counter;

        if (stack->top == NULL)
          {
            return -1;
          }

        newcounter = add(stack->counter, 1) | (uint64_t)stack->top->next;
        //new_top = stack->top->next;
        *elem = stack->top;
        stack->top = stack->top->next;
        /*
         * We can't fix the ABA problem now, as this would necessitate the use of a 64 bits CAS
         * using two 32 bits registers (one for the address to swap, the second for the counter)
         * Without this double 32-bits register CAS, it is impossible to safely swap a 64bits
         * counter & mask AND the address to swap at the same time.
         *
         * Unfortunately, we don't have such double CAS.
         */
      }while (!cas64(&stack->counter, &oldcounter, &newcounter, &stack->lock));
#else
    do
      {
        old_top = stack->top;
        *elem = stack->top;
      }
    while (!cas(&stack->top, &old_top, &old_top->next, &stack->lock));
#endif

    (*elem)->next = NULL;
#endif

    return 0;
  }

int
pelib_stack_push_safe_managed(STACK_T)(stack_t(STACK_T) *s, stack_t(STACK_T) *pool, STACK_T buffer)
  {
    stackelem_t(STACK_T) *elem = NULL;

    assert(pelib_stack_check(STACK_T)(pool) == 0);

    if (pelib_stack_pop_elem(STACK_T)(pool, &elem) != 0)
      {
        elem = malloc(sizeof(stackelem_t(STACK_T)));
        pelib_init(stackelem_t(STACK_T))(elem);
      }

    pelib_stackelem_write(STACK_T)(elem, buffer);

    pelib_stack_push_elem_safe(STACK_T)(s, elem);

    return 0;
  }

int
pelib_stack_pop_safe_managed(STACK_T)(stack_t(STACK_T) *s, stack_t(STACK_T) *pool, STACK_T* buffer)
  {
    stackelem_t(STACK_T) *elem = NULL;

    if (pelib_stack_pop_elem_safe(STACK_T)(s, &elem) != 0)
      {
        return -1;
      }
    pelib_stackelem_read(STACK_T)(elem, buffer);

    pelib_stack_push_elem(STACK_T)(pool, elem);

    return 0;
  }

#undef STACK_T
