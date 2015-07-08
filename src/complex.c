/*
 * complex.c
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

#include <pelib/pelib.h>

#include <pelib/complex.h>

int
pelib_copy
( complex_t)(complex_t s1, complex_t * s2)
{
  s2->r = s1.r;
  s2->im = s1.im;

  return PELIB_SUCCESS;
}

int
pelib_init
( complex_t)(complex_t *cplx)
{
  cplx->im = 0;
  cplx->r = 0;

  return PELIB_SUCCESS;
}

int
pelib_compare
( complex_t)(complex_t a, complex_t b)
{
  if (a.r == b.r)
    {
      return a.im - b.im;
    }
  else
    {
      return a.r - b.r;
    }
}

// Now include the generic stack implementation
#define STACK_T complex_t
#include <pelib/stack.c>
