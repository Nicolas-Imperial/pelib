/*
 * iterator.c
 *
 *  Created on: 5 Sep 2011
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef ITERATOR_T
#error Using generic iterator without a type
#endif

iterator_t(ITERATOR_T)*
pelib_alloc_struct(iterator_t(ITERATOR_T))()
{
	iterator_t(ITERATOR_T)* iterator;

	iterator = malloc(sizeof(iterator_t(ITERATOR_T)));
	assert(iterator != NULL);

	return iterator;
}

iterator_t(ITERATOR_T)*
pelib_alloc(iterator_t(ITERATOR_T))()
{
	iterator_t(ITERATOR_T)* iterator;

	iterator = malloc(sizeof(iterator_t(ITERATOR_T)));
	assert(iterator != NULL);

	return iterator;
}

int
pelib_init(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T)* iterator)
{
	iterator->previous = NULL;
	iterator->next = NULL;
	return pelib_init(ITERATOR_T)(&iterator->value);
}

int
pelib_copy(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T) src, iterator_t(ITERATOR_T)* dst)
{
	size_t i;

	*dst = src;
	return pelib_copy(ITERATOR_T)(src.value, &dst->value);
}

#define iterator_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define iterator_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_iterator_length(ITERATOR_T)(&iterator));
char*
pelib_string(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T) iterator)
{
	char *value = pelib_string(ITERATOR_T)(iterator.value);
	char *elem = malloc((strlen(value) + 3) * sizeof(char*));
	strcpy(&elem[1], value);
	elem[0] = '{';
	elem[strlen(value)] = '}';
	elem[strlen(value) + 1] = '\0';
	free(value);

	return elem;
}

char*
pelib_string_detail(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T) iterator, int level)
{
	if(level == 0)
	{
		return pelib_string(iterator_t(ITERATOR_T))(iterator);
	}
	else
	{
		char *value = pelib_string_detail(ITERATOR_T)(iterator.value, level);
		char *elem = malloc((strlen(value) + 3) * sizeof(char*));
		strcpy(&elem[1], value);
		elem[0] = '{';
		elem[strlen(value)] = '}';
		elem[strlen(value) + 1] = '\0';
		free(value);

		return elem;
	}
}

FILE*
pelib_printf(iterator_t(ITERATOR_T))(FILE* stream, iterator_t(ITERATOR_T) iterator)
{
	char * str;
	str = pelib_string(iterator_t(ITERATOR_T))(iterator);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(iterator_t(ITERATOR_T))(FILE* stream, iterator_t(ITERATOR_T) iterator, int level)
{
	char * str;
	str = pelib_string_detail(iterator_t(ITERATOR_T))(iterator, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_destroy(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T) iterator)
{
	pelib_destroy(ITERATOR_T)(iterator.value);
	return 1;
}

int
pelib_free(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T)* iterator)
{
	return pelib_free_struct(iterator_t(ITERATOR_T))(iterator);
}

int
pelib_free_struct(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T)* iterator)
{
	pelib_destroy(ITERATOR_T)(iterator->value);
	free(iterator);
	return 0;
}

int
pelib_compare(iterator_t(ITERATOR_T))(iterator_t(ITERATOR_T) a1, iterator_t(ITERATOR_T) a2)
{
	return pelib_compare(ITERATOR_T)(a1.value, a2.value);
}

iterator_t(ITERATOR_T)*
pelib_iterator_next(ITERATOR_T)(iterator_t(ITERATOR_T) *el)
{
	if(el != NULL)
	{
		return el->next;
	}
	else
	{
		return NULL;
	}
}

ITERATOR_T
pelib_iterator_read(ITERATOR_T)(iterator_t(ITERATOR_T) *el)
{
	return el->value;
}

#undef ITERATOR_T
