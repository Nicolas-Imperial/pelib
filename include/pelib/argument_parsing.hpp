/*
 Copyright 2015 Nicolas Melot

 This file is part of Pelib.

 Pelib is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pelib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Pelib. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PELIB_ARGUMENT_PARSING_H
#define PELIB_ARGUMENT_PARSING_H

enum stream {STREAM_NOTHING, STREAM_STDIN, STREAM_STDOUT, STREAM_STDERR, STREAM_FILE};

struct pelib_argument_stream
{
	char *library;
	char **argv;
	size_t argc;
	char *filename;
	char *name;
	enum stream stream;
};
typedef struct pelib_argument_stream pelib_argument_stream_t;

void pelib_argument_stream_init(pelib_argument_stream_t *pelib_argument_stream);

void pelib_argument_stream_destroy(const pelib_argument_stream_t pelib_argument_stream);

unsigned int
pelib_argument_stream_parse(char **argv, pelib_argument_stream_t* stream);

#endif

