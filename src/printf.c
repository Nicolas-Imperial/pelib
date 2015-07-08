/*
 * printf.c
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

#include <stdarg.h>
#include <pelib/printf.h>

FILE*
(*in_admin)();
FILE*
(*out_admin)();
FILE*
(*err_admin)();

static FILE*(*get_stream(FILE* (*try)(), FILE* (*subst)()))
()
{
	if (try == NULL)
	{
		return subst;
	}
	else
	{
		return try;
	}
}

static FILE*
get_stdin()
{
	return stdin;
}

static FILE*
get_stdout()
{
	return stdout;
}

static FILE*
get_stderr()
{
	return stderr;
}

FILE*
pelib_get_stdin()
{
	return get_stream(in_admin, &get_stdin)();
}
FILE*
pelib_get_stdout()
{
	return get_stream(out_admin, &get_stdout)();
}
FILE*
pelib_get_stderr()
{
	return get_stream(err_admin, &get_stderr)();
}

int
pelib_set_stdinfunc(FILE*
(*func)())
{
	in_admin = func;

	return 1;
}

int
pelib_set_stdoutfunc(FILE*
(*func)())
{
	out_admin = func;

	return 1;
}

int
pelib_set_stderrfunc(FILE*
(*func)())
{
	err_admin = func;

	return 1;
}

void
pelib_stdprintf(const char* format, ...)
{
	va_list args;
 	va_start(args, format);
 	vfprintf(pelib_get_stdout(), format, args);
	va_end(args);
}

void
pelib_errprintf(const char* format, ...)
{
	va_list args;
 	va_start(args, format);
 	vfprintf(pelib_get_stderr(), format, args);
	va_end(args);
}
