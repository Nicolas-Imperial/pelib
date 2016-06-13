/*
 * map.c
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

#if !(defined MAP_KEY_T && defined MAP_VALUE_T)
#error Using generic map without a type
#endif

/*
#define PAIR_KEY_T MAP_KEY_T
#define PAIR_VALUE_T MAP_VALUE_T
#include "pelib/pair.c"
*/

/*
#define ITERATOR_T pair_t(MAP_KEY_T, MAP_VALUE_T)
#include "pelib/iterator.c"
*/

#if 10
#define debug(var) printf("[%s:%s:%d] %s = \"%s\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_addr(var) printf("[%s:%s:%d] %s = \"%p\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_int(var) printf("[%s:%s:%d] %s = \"%d\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_size_t(var) printf("[%s:%s:%d] %s = \"%zu\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#else
#define debug(var)
#define debug_addr(var)
#define debug_int(var)
#define debug_size_t(var)
#endif

static map_t(MAP_KEY_T, MAP_VALUE_T) *stuff;

map_t(MAP_KEY_T, MAP_VALUE_T)*
pelib_alloc_struct(map_t(MAP_KEY_T, MAP_VALUE_T))()
{
	map_t(MAP_KEY_T, MAP_VALUE_T)* map;

	map = malloc(sizeof(map_t(MAP_KEY_T, MAP_VALUE_T)));
	assert(map != NULL);

	return map;
}

map_t(MAP_KEY_T, MAP_VALUE_T)*
pelib_alloc(map_t(MAP_KEY_T, MAP_VALUE_T))()
{
	map_t(MAP_KEY_T, MAP_VALUE_T)* map;

	map = malloc(sizeof(map_t(MAP_KEY_T, MAP_VALUE_T)));
	assert(map != NULL);

	return map;
}

int
pelib_init(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T)* map)
{
	map->first = NULL;
	map->middle = NULL;
	map->last = NULL;

	return 1;
}

#define pelib_map_insert_element(key, value) PELIB_CONCAT_3(pelib_, map(key, value), _insert_element)
static
int
pelib_map_insert_element(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T) *map, iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *elem)
{
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *current = map->first, *previous = NULL;

	// If there is at least one element, then browse the list until we find the right spot
	// Otherwise, place the element as first and last, that is first and last pointers point
	// the the element and next and previous pointers of this element point to nothing.
	if(current != NULL)
	{
		// browse the list until we reach the end or until we find the last element lower 
		// than the element to insert
		while(current != NULL && pelib_compare(MAP_KEY_T)(elem->value.key, current->value.key) > 0)
		{
			previous = current;
			current = current->next;
		}

		/* Comment out the next block if you want to implement a multimap */
		if(current != 0 && pelib_compare(MAP_KEY_T)(elem->value.key, current->value.key) == 0)
		{
			return 0;
		}

		// Insert element
		elem->previous = previous;
		elem->next = current;
		if(previous != NULL)
		{
			previous->next = elem;
		}

		// Update tail if the element is insert last
		// Otherwise update next element's pointer to
		// previous element (the newly inserted one)
		if(current == NULL)
		{
			map->last = elem;
		}
		else
		{
			if(current == map->first)
			{
				map->first = elem;
			}
			current->previous = elem;
		}
	}
	else
	{
		elem->next = NULL;
		elem->previous = NULL;
		map->first = elem;
		map->last = elem;
	}

	return 1;
}

int
pelib_map_insert(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T)* map, pair_t(MAP_KEY_T, MAP_VALUE_T) value)
{
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *new = pelib_alloc(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))();
	pelib_init(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(new);
	pelib_copy(pair_t(MAP_KEY_T, MAP_VALUE_T))(value, &new->value);
	if(!pelib_map_insert_element(MAP_KEY_T, MAP_VALUE_T)(map, new))
	{
		pelib_free(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(new);
		return 0;
	}
	else
	{
		return 1;
	}
}

int
pelib_copy(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T) src, map_t(MAP_KEY_T, MAP_VALUE_T)* dst)
{
	size_t i;

	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *elem = src.last;
	while(elem != NULL)
	{
		iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *cpy = pelib_alloc(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))();
		pelib_copy(pair_t(MAP_KEY_T, MAP_VALUE_T))(elem->value, &cpy->value);
		pelib_map_insert_element(MAP_KEY_T, MAP_VALUE_T)(dst, cpy);
		elem = elem->previous;
	}

	return 0;
}

#define map_length_debug printf("[PELIB:%s:%s:%d] i = %d\n", __FILE__, __FUNCTION__, __LINE__, i);
#define map_length_pre_debug printf("[PELIB:%s:%s:%d] length = %d\n", __FILE__, __FUNCTION__, __LINE__, pelib_map_length(MAP_KEY_T, MAP_VALUE_T)(&map));
char*
pelib_string(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T) map)
{
	char *str, *grow, *elem;
	size_t i;
	str = malloc(3 * sizeof(char));
	sprintf(str, "[");

	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *el = map.first;
	while(el != NULL && el->next != NULL)
	{
		elem = pelib_string(pair_t(MAP_KEY_T, MAP_VALUE_T))(el->value);
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s:", grow, elem);
		free(elem);
		free(grow);
		el = el->next;
	}
	while(el != NULL)
	{
		elem = pelib_string(pair_t(MAP_KEY_T, MAP_VALUE_T))(el->value);
		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);

		free(str);
		str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
		sprintf(str, "%s%s", grow, elem);
		free(elem);
		free(grow);

		el = el->next;
	}

	grow = malloc((strlen(str) + 1) * sizeof(char));
	sprintf(grow, "%s", str);
	sprintf(str, "%s]", grow);
	free(grow);

	return str;
}

char*
pelib_string_detail(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T) map, int level)
{
	if(level == 0)
	{
		return pelib_string(map_t(MAP_KEY_T, MAP_VALUE_T))(map);
	}
	else
	{
		// Bring it on
		char *str, *grow, *elem;
		size_t i;
		str = malloc(3 * sizeof(char));
		sprintf(str, "[");
		iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *el = map.first;
		
		while(el != NULL && el->next != NULL)
		{
			elem = pelib_string_detail(pair_t(MAP_KEY_T, MAP_VALUE_T))(el->value, level);
			grow = malloc((strlen(str) + 1) * sizeof(char));
			sprintf(grow, "%s", str);

			free(str);
			str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
			sprintf(str, "%s%s:", grow, elem);
			free(elem);
			free(grow);
		}
		while(el != NULL)
		{
			elem = pelib_string_detail(pair_t(MAP_KEY_T, MAP_VALUE_T))(el->value, level);
			grow = malloc((strlen(str) + 1) * sizeof(char));
			sprintf(grow, "%s", str);

			free(str);
			str = malloc(strlen(grow) + 1 + strlen(elem) + 1);
			sprintf(str, "%s%s", grow, elem);
			free(elem);
			free(grow);
		}

		grow = malloc((strlen(str) + 1) * sizeof(char));
		sprintf(grow, "%s", str);
		sprintf(str, "%s]", grow);
		free(grow);

		return str;
	}
}

FILE*
pelib_printf(map_t(MAP_KEY_T, MAP_VALUE_T))(FILE* stream, map_t(MAP_KEY_T, MAP_VALUE_T) map)
{
	char * str;
	str = pelib_string(map_t(MAP_KEY_T, MAP_VALUE_T))(map);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(map_t(MAP_KEY_T, MAP_VALUE_T))(FILE* stream, map_t(MAP_KEY_T, MAP_VALUE_T) map, int level)
{
	char * str;
	str = pelib_string_detail(map_t(MAP_KEY_T, MAP_VALUE_T))(map, level);

	fprintf(stream, "%s\n", str);
	free(str);

	return stream;
}

int
pelib_destroy(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T) map)
{
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *elem = map.first;
	while(elem != NULL)
	{
		iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *current = elem;
		elem = elem->next;
		pelib_destroy(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(*current);
		pelib_free(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(current);		
	}
	return 1;
}

int
pelib_free_buffer(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T)* map)
{
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *elem = map->first;
	while(elem != NULL)
	{
		iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *current = elem;
		elem = elem->next;
		pelib_destroy(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(*current);
		pelib_free(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)))(current);		
	}
	return 1;
}

int
pelib_free_struct(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T)* map)
{
	free(map);
	return 1;
}

int
pelib_free(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T)* map)
{
	pelib_free_buffer(map_t(MAP_KEY_T, MAP_VALUE_T))(map);
	pelib_free_struct(map_t(MAP_KEY_T, MAP_VALUE_T))(map);
	return 1;
}

int
pelib_compare(map_t(MAP_KEY_T, MAP_VALUE_T))(map_t(MAP_KEY_T, MAP_VALUE_T) a1, map_t(MAP_KEY_T, MAP_VALUE_T) a2)
{
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *e1 = a1.first;
	iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *e2 = a2.first;

	while(e1 != NULL && e2 != NULL && pelib_compare(pair_t(MAP_KEY_T, MAP_VALUE_T))(e1->value, e2->value) == 0)
	{
		e1 = e1->next;
		e2 = e2->next;
	}

	if(e1 == NULL && e2 != NULL)
	{
		return 1;
	}
	else if(e1 != NULL && e2 == NULL)
	{
		return -1;
	}
	else return pelib_compare(pair_t(MAP_KEY_T, MAP_VALUE_T))(e1->value, e2->value);
}

/* Returns the first element in map */
iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_begin(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T) *map)
{
	return map->first;
}

/* Returns the first element in map */
iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_end(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T) *map)
{
	return NULL;
}

iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_next(MAP_KEY_T, MAP_VALUE_T)(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *i)
{
	return pelib_iterator_next(pair_t(MAP_KEY_T, MAP_VALUE_T))(i);
}

pair_t(MAP_KEY_T, MAP_VALUE_T)
pelib_map_read(MAP_KEY_T, MAP_VALUE_T)(iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T)) *i)
{
	return pelib_iterator_read(pair_t(MAP_KEY_T, MAP_VALUE_T))(i);
}

iterator_t(pair_t(MAP_KEY_T, MAP_VALUE_T))*
pelib_map_find(MAP_KEY_T, MAP_VALUE_T)(map_t(MAP_KEY_T, MAP_VALUE_T) *map, MAP_KEY_T key)
{
	map_iterator_t(MAP_KEY_T, MAP_VALUE_T) *i;
	for(i = pelib_map_begin(MAP_KEY_T, MAP_VALUE_T)(map); i != pelib_map_end(MAP_KEY_T, MAP_VALUE_T)(map); i = pelib_map_next(MAP_KEY_T, MAP_VALUE_T)(i))
	{
		if(pelib_compare(MAP_KEY_T)(key, pelib_map_read(MAP_KEY_T, MAP_VALUE_T)(i).key) == 0)
		{
			return i;
		}
	}
}

#undef MAP_KEY_T
#undef MAP_VALUE_T
