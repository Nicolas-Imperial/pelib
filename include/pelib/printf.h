/*
 * printf.h
 *
 *  Created on: 19 Jan 2012
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

#ifndef PRINTF_H_
#define PRINTF_H_

FILE* pelib_get_stdin();
FILE* pelib_get_stdout();
FILE* pelib_get_stderr();

int
pelib_set_stdinfunc(FILE* (*func)());

int pelib_set_stdoutfunc(FILE* (*func)());

int pelib_set_stderrfunc(FILE* (*func)());

void pelib_stdprintf(const char*, ...);
void pelib_errprintf(const char*, ...);

#endif /* PRINTF_H_ */
