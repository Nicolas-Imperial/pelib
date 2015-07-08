/*
 * integer.h
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

#ifndef INTEGER_H_
#define INTEGER_H_

#define STRUCT_T int
#include <pelib/structure.h>
#define DONE_int

#define ARRAY_T int
#include <pelib/array.h>
#define DONE_array_int

#define CFIFO_T int
#include <pelib/fifo.h>
#define DONE_cfifo_int

#define CFIFO_ARRAY_T int
#include <pelib/fifo_array.h>
#define DONE_cfifo_array_int

#endif /* INTEGER_H_ */
