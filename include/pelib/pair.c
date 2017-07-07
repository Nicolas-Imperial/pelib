/*
 * pair.c
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

#if !(defined PAIR_KEY_T && defined PAIR_VALUE_T)
#error Using generic pair without key or value types
#endif

pair_t(PAIR_KEY_T, PAIR_VALUE_T)*
pelib_alloc_struct(pair_t(PAIR_KEY_T, PAIR_VALUE_T))()
{
	pair_t(PAIR_KEY_T, PAIR_VALUE_T)* pair;

	pair = malloc(sizeof(pair_t(PAIR_KEY_T, PAIR_VALUE_T)));
	assert(pair != NULL);

	return pair;
}

int
pelib_init(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T)* pair)
{
	int a = pelib_init(PAIR_KEY_T)(&pair->key);
	return a && pelib_init(PAIR_VALUE_T)(&pair->value);
}

int
pelib_copy(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T) src, pair_t(PAIR_KEY_T, PAIR_VALUE_T)* dst)
{
	int a = pelib_copy(PAIR_KEY_T)(src.key, &dst->key);	
	return a && pelib_copy(PAIR_VALUE_T)(src.value, &dst->value);
}

#define pair_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define pair_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_pair_length(PAIR_KEY_T, PAIR_VALUE_T)(&pair));
char*
pelib_string(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T) pair)
{
	char *key = pelib_string(PAIR_KEY_T)(pair.key);
	char *value = pelib_string(PAIR_VALUE_T)(pair.value);

	char *elem = malloc((strlen(key) + strlen(value) + 4) * sizeof(char*));
	elem[0] = '(';
	strcpy(&elem[1], key);
	elem[1 + strlen(key)] = ',';
	strcpy(&elem[1 + strlen(key) + 1], value);
	elem[1 + strlen(key) + 1 + strlen(value)] = ')';
	elem[1 + strlen(key) + 1 + strlen(value) + 1] = '\0';
	free(key);
	free(value);

	return elem;
}

char*
pelib_string_detail(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T) pair, int level)
{
	if(level == 0)
	{
		return pelib_string(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair);
	}
	else
	{
		char *key = pelib_string(PAIR_KEY_T)(pair.key);
		char *value = pelib_string(PAIR_VALUE_T)(pair.value);

		char *elem = malloc((strlen(key) + strlen(value) + 4) * sizeof(char*));
		elem[0] = '(';
		strcpy(&elem[1], key);
		elem[1 + strlen(key)] = ',';
		strcpy(&elem[1 + strlen(key) + 1], value);
		elem[1 + strlen(key) + 1 + strlen(value)] = ')';
		elem[1 + strlen(key) + 1 + strlen(value) + 1] = '\0';
		free(key);
		free(value);

		return elem;
	}
}

FILE*
pelib_printf(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(FILE* stream, pair_t(PAIR_KEY_T, PAIR_VALUE_T) pair)
{
	char * str;
	str = pelib_string(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(FILE* stream, pair_t(PAIR_KEY_T, PAIR_VALUE_T) pair, int level)
{
	char * str;
	str = pelib_string_detail(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_free(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T)* pair)
{
	return pelib_free_struct(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair);
}

int
pelib_free_struct(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T)* pair)
{
	pelib_destroy(PAIR_KEY_T)(pair->key);
	pelib_destroy(PAIR_VALUE_T)(pair->value);
	free(pair);
	return 0;
}

int
pelib_compare(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T) a1, pair_t(PAIR_KEY_T, PAIR_VALUE_T) a2)
{
	int a = pelib_compare(PAIR_KEY_T)(a1.key, a2.key);
	int b = pelib_compare(PAIR_VALUE_T)(a1.value, a2.value);
	return a == 0 ? b : a;
}

int
pelib_destroy(pair_t(PAIR_KEY_T, PAIR_VALUE_T))(pair_t(PAIR_KEY_T, PAIR_VALUE_T) set)
{
	pelib_destroy(PAIR_KEY_T)(set.key);
	pelib_destroy(PAIR_VALUE_T)(set.value);
	return 1;
}

#undef PAIR_KEY_T
#undef PAIR_VALUE_T
