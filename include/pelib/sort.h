/*
 * sort.h
 *
 *  Created on: 25 Jan 2012
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

#ifndef SORT_H_
#define SORT_H_

#include <pelib/integer.h>

#define PELIB_QUICKSORT_THRESHOLD 15
#define PELIB_QUICKSORT_NUM_SAMPLE 10

struct qsort_tune
{
	int begin;
	int end;
	int threshold;
	int num_sample;
};

typedef struct qsort_tune qsort_tune_t;

int
pelib_sample(array_t(int)*, int, int, int);
void
pelib_insertsort_window(array_t(int)*, int, int);
void
pelib_insertsort(array_t(int)*);
void
pelib_quicksort_tune(array_t(int)* array, qsort_tune_t p);
void
pelib_quicksort_tune_recursive(array_t(int)* array, qsort_tune_t p);
void
pelib_quicksort_window(array_t(int)* array, int, int);
void
pelib_quicksort(array_t(int)*);
int
pelib_is_increasing(array_t(int)*);

#endif /* SORT_H_ */
