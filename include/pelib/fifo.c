/*
 * fifo.c
 *
 *  Created on: 26 Jan 2012
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
//#ifndef UNUSED
//#define UNUSED(x) (void)(x)
//#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include <pelib/printf.h>

#ifndef CFIFO_T
#error Using generic fifo without a type
#endif

#define CFIFO_EMPTY "[]"
#define CFIFO_SEPARATOR ":"
#define CFIFO_BEGIN "["
#define CFIFO_END "]"

#ifndef debug
#if DEBUG
#define printf_addr(addr) printf("[%s:%s:%d] @%s: %08X\n", __FILE__, __FUNCTION__, __LINE__, #addr, addr);
#define printf_int(addr) printf("[%s:%s:%d] %s: %d (signed) = %u (unsigned)\n", __FILE__, __FUNCTION__, __LINE__, #addr, addr, addr);
#define debug printf("[%s:%s:%d] Hello world!\n", __FILE__, __FUNCTION__, __LINE__);
#else
#define printf_addr(addr)
#define printf_int(var)
//#define printf_int(addr) printf("[%s:%s:%d] %s: %d (signed) = %u (unsigned)\n", __FILE__, __FUNCTION__, __LINE__, #addr, addr, addr);
#define debug
#endif
#endif

enum cfifo_state
{
  NORMAL, REVERSE, EMPTY, FULL
};
typedef enum cfifo_state cfifo_state_t;

#if 0
CFIFO_T
pelib_fifo_elem_init()
  {
    CFIFO_T elem;
    elem = 0;

    return elem;
  }

char*
pelib_fifo_elem_string(CFIFO_T elem)
  {
    char *str;

    str = malloc((PELIB_FIFO_ELEM_MAX_CHAR + 1) * sizeof(char));

    sprintf(str, "%i", elem);

    return str;
  }
#endif

/*
#define get_write(elem) PELIB_CONCAT_3(get_, elem, _write)
static
size_t
get_write(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
{
	size_t write;
	write = (cfifo->id < 0) ? cfifo->write : *(size_t*)cfifo->write;
//	printf_int(write);
	return write;
}
*/

int
pelib_alloc_buffer(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* cfifo, size_t capacity)
{
	/*if(init.core < 0)
	{*/
		cfifo->buffer = (CFIFO_T*)malloc(sizeof(CFIFO_T) * capacity);
	/*}
	else
	{
		cfifo->buffer = (CFIFO_T*)pelib_scc_global_ptr(pelib_scc_stack_grow(init.stack, sizeof(CFIFO_T) * capacity, init.core), init.core);
//		printf_int(pelib_scc_local_offset((void*)cfifo->buffer, cfifo->id));
	}*/
	cfifo->capacity = capacity;

	return 1;
}

cfifo_t(CFIFO_T)*
pelib_alloc_struct(cfifo_t(CFIFO_T))()
{
	cfifo_t(CFIFO_T) *fifo;
	fifo = (cfifo_t(CFIFO_T)*)malloc(sizeof(cfifo_t(CFIFO_T)));

	return fifo;
}

cfifo_t(CFIFO_T)*
pelib_alloc_collection(cfifo_t(CFIFO_T))(size_t size)
{
	cfifo_t(CFIFO_T) *fifo;
	fifo = pelib_alloc_struct(cfifo_t(CFIFO_T))();
	if(fifo != NULL)
	{
		
	//fifo->id = init.core;

//	if(init.core >= 0)
//	{
//		printf_int(fifo->id);
//		printf("[%s:%s:%d] Fifo @%X: Allocate write to MPB@ %X\n", __FILE__, __FUNCTION__, __LINE__, fifo, pelib_scc_local_offset((void*)fifo->write, init.core));
//		fifo->write = (size_t)pelib_scc_global_ptr(pelib_scc_stack_grow(init.stack, sizeof(size_t), init.core), init.core);
//		printf_int(pelib_scc_local_offset((void*)fifo->write, fifo->id));
//		printf("[%s:%s:%d] Fifo @%X: Allocate write to MPB@ %X\n", __FILE__, __FUNCTION__, __LINE__, fifo, pelib_scc_local_offset((void*)fifo->write, init.core));
//	}

		if(pelib_alloc_buffer(cfifo_t(CFIFO_T))(fifo, size) == 0)
		{
			pelib_free_struct(cfifo_t(CFIFO_T))(fifo);
			return NULL;
		}
	}
		

	return fifo;
}

int
pelib_init(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	fifo->last_op = PELIB_CFIFO_POP;
	fifo->read = 0;

//	if(fifo->id < 0)
//	{
		fifo->write = 0;
//	}
//	else
//	{
//		// TODO: check
//		pelib_scc_cache_invalidate();
//		*(size_t*)fifo->write = 0;
//		printf_int(*(size_t*)fifo->write);
//	};

	return 1;
}

int
pelib_free(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	pelib_free_buffer(cfifo_t(CFIFO_T))(fifo);
	return pelib_free_struct(cfifo_t(CFIFO_T))(fifo);
}

int
pelib_free_struct(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	free((void*)fifo->buffer);

	return 1;
}

int
pelib_free_buffer(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T)* fifo)
{
	free(fifo);
	return 1;
}

FILE*
pelib_printf(cfifo_t(CFIFO_T))(FILE* stream, cfifo_t(CFIFO_T) fifo)
{
	char* str;
	str = pelib_string(cfifo_t(CFIFO_T))(fifo);
	printf("%s\n", str);
	free(str);

	return stream;
}

FILE*
pelib_printf_detail(cfifo_t(CFIFO_T))(FILE* stream, cfifo_t(CFIFO_T) fifo, int level)
{
	char* str;
	str = pelib_string_detail(cfifo_t(CFIFO_T))(fifo, level);
	printf("%s\n", str);
	free(str);

	return stream;
}

#if 0
#define check_memory(elem) PELIB_CONCAT_3(check_, elem, _memory)
static
int
check_memory(CFIFO_T)(CFIFO_T* ptr, size_t length)
{
	size_t i;
	for(i = 0; i < length; i++)
	{
	  //if(ptr[i] == (CFIFO_T)MARKER && 0)
	  if(ptr[i] == MARKER)
		{
		  //printf("MARKER FOUND: buffer is %i\n",*(int*)ptr);
		  //exit(65);
			return 0;
		}
	}

	return 1;
}
#else
#define check_memory(elem)
#endif

#define cfifo_state(elem) PELIB_CONCAT_3(cfifo_, cfifo(elem), _state)
static cfifo_state_t
cfifo_state(CFIFO_T)(cfifo_t(CFIFO_T) *cfifo)
{
//	printf("%X\n", cfifo);
  //int id = cfifo->id;
/*
	if(id < 0)
	{
		write = cfifo->write;
	}
	else
	{
//		printf("%X\n", cfifo->write);
		write = *(size_t*)cfifo->write;
	}
//    volatile size_t write = get_write(CFIFO_T)(scc_fifo);
*/

    if (cfifo->read == cfifo->write)
      {
        if (cfifo->last_op == PELIB_CFIFO_POP)
          {
            return EMPTY;
          }
        else if (cfifo->last_op == PELIB_CFIFO_PUSH)
          {
            return FULL;
          }
        else
          {
            fprintf(stderr, "[ERROR:%s:%s:%i] Illegal cfifo state\n", __FILE__, __FUNCTION__, __LINE__);
            return EMPTY;
          }
      }
    else if (cfifo->read < cfifo->write)
      {
        return NORMAL;
      }
    else
      {
        return REVERSE;
      }
  }

#define is_in_content(elem) PELIB_CONCAT_2(cfifo(elem), _is_in_content)
// returns 1 if index is in data area
// returns 3 if index is in position of read index
// returns 7 if index is in both read and write position, and in data area (fifo full)
// returns -7 if index is in both read and write position not in data area (fifo empty)
// returns -1 if index is not in data area
// returns -5 if index is in position of write index but not in data area
// ERRORS:
// returns -2 if index is in position of read index but not read, but is not in data area
// returns 5 if index is in position of write index but not read, but still is in data area
// returns 6 if in both read and write indexes but cannot determine if in data area or not
// returns 0 in case of other errors
/**
 * Returns an integer whose bits represent (if set):
 * 0: always set to 1
 * 1: index in position of read index
 * 2: index in position of write index
 * value is made negative if index is not in data area
 */
static int
is_in_content(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, size_t index)
  {
    int res;
    size_t read;
    cfifo_state_t state;

    read = fifo->read;
    state = cfifo_state(CFIFO_T)(fifo);

    res = 1;
    res += index == read ? 2 : 0;
    res += index == fifo->write ? 4 : 0;

    switch (state)
      {
        case NORMAL:
        if (index < read || index >= fifo->write)
          {
            res = -1 * res;
          }
        break;
        case REVERSE:
        if (index >= fifo->write && index < read)
          {
            res = -1 * res;
          }
        break;
        case EMPTY:
        res = -1 * res;
        break;
        case FULL:
        // Do nothing
        break;
        default:
        fprintf(stderr, "[ERROR:%s:%s:%i] Illegal cfifo state", __FILE__, __FUNCTION__, __LINE__);
        return 0;
        break;
      }

    return res;
  }

char*
pelib_string(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T) fifo)
  {
    return pelib_string_detail(cfifo_t(CFIFO_T))(fifo, 0);
  }

char*
pelib_string_detail(cfifo_t(CFIFO_T))(cfifo_t(CFIFO_T) fifo, int level)
{
  char *str, *elem;
  unsigned int i;
  int status;

  str = (char*)malloc(sizeof(char) * ((PELIB_FIFO_ELEM_MAX_CHAR + 1) * fifo.capacity)
      + 4);

  sprintf(str, "[");
  for (i = 0; i < fifo.capacity; i++)
  {
    status = is_in_content(CFIFO_T)(&fifo, i);
    if (abs(status) & 2)
    {
      sprintf(str, "%s>", str);
    }
    if (abs(status) & 4)
    {
      sprintf(str, "%s>", str);
    }
    if(status > 0)
    {
      elem = pelib_string_detail(CFIFO_T)(fifo.buffer[i], level);
      sprintf(str, "%s%s", str, elem);
      free(elem);
    }
    else
    {
      sprintf(str, "%s.", str);
    }
    if (i < fifo.capacity - 1)
    {
      sprintf(str, "%s:", str);
    }
  }
  sprintf(str, "%s]", str);

  return str;
}

#if 0
void
pelib_cfifo_printf_content(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
  {
    char* str;
    str = pelib_cfifo_string_content(CFIFO_T)(fifo);
    fprintf(pelib_get_stdout(), "%s\n", str);
    free(str);
  }

char*
pelib_cfifo_string_detail(CFIFO_T)
(cfifo_t(CFIFO_T)* cfifo)
  {
    char *str, *elem;
    unsigned int i;
    int length;
    cfifo_state_t state;

    state = cfifo_state(CFIFO_T)(cfifo);

    switch (state)
      {
        case NORMAL:
        length = ((unsigned int) cfifo->write_ptr
            - (unsigned int) cfifo->read_ptr) / sizeof(CFIFO_T);
        break;
        case REVERSE:
        length = ((unsigned int) cfifo->read_ptr
            - (unsigned int) cfifo->write_ptr)
        / sizeof(CFIFO_T);
        break;
        case EMPTY:
        str = malloc(sizeof(CFIFO_EMPTY) + 1);
        sprintf(str, CFIFO_EMPTY);
        return str;
        break;
        case FULL:
        length = cfifo->capacity;
        break;
        default:
        fprintf(pelib_get_stderr(),
            "Illegal circular fifo state: %i\n", state);
        break;
      }

    str = malloc((PELIB_FIFO_ELEM_MAX_CHAR
            + sizeof(CFIFO_SEPARATOR)) * length + sizeof(CFIFO_BEGIN)
        + sizeof(CFIFO_END) + 1);

    sprintf(str, "%s", CFIFO_BEGIN);
    int end = ((unsigned int) cfifo->write_ptr
        - (unsigned int) cfifo->buffer) / sizeof(CFIFO_T);
    end = end + cfifo->capacity - 1;
    end = end % cfifo->capacity;
    for (i = ((unsigned int) cfifo->read_ptr
            - (unsigned int) cfifo->buffer) / sizeof(CFIFO_T); i
        != end; i = (i + 1) % cfifo->capacity)
      {
        elem = pelib_string(CFIFO_T)(cfifo->buffer[i]);
        sprintf(str, "%s%s%s", str, elem, CFIFO_SEPARATOR);
      }
    elem = pelib_string(CFIFO_T)(cfifo->buffer[i]);
    sprintf(str, "%s%s%s", str, elem, CFIFO_END);

    return str;
  }
#endif

int
pelib_cfifo_push(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, CFIFO_T elem)
  {
    if (fifo->capacity > 0 && 
	cfifo_state(CFIFO_T)(fifo) != FULL)
      {
        memcpy((void*)&(fifo->buffer[fifo->write]), &elem, sizeof(CFIFO_T));

        fifo->write = (fifo->write + 1) % fifo->capacity;
        fifo->last_op = PELIB_CFIFO_PUSH;

        return 1;
      }
    else
      {
        return 0;
      }
  }

size_t
pelib_cfifo_fill(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, size_t num)
{
	size_t length = pelib_cfifo_capacity(CFIFO_T)(fifo) - pelib_cfifo_length(CFIFO_T)(*fifo);
	num = length < num ? length : num;

	fifo->write = (fifo->write + num) % fifo->capacity;

	if(num > 0)
	{
		fifo->last_op = PELIB_CFIFO_PUSH;
	}

	return num;
}

#define cfifo_peek(elem) PELIB_CONCAT_3(cfifo_, cfifo(elem), _peek)
static
CFIFO_T*
cfifo_peek(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, size_t offset)
  {
    if (fifo->capacity > 0 &&
	offset < pelib_cfifo_length(CFIFO_T)(*fifo) &&
	cfifo_state(CFIFO_T)(fifo) != EMPTY)
      {
        return (CFIFO_T*)&(fifo->buffer[(fifo->read + offset) % fifo->capacity]);
      }
    else
      {
        return NULL;
      }
  }

/*
CFIFO_T
pelib_cfifo_last(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
{
//	pelib_printf(cfifo_t(CFIFO_T))(*fifo);
	if(!pelib_cfifo_is_empty(CFIFO_T)(fifo))
	{
#if 0
#define printf_addr(addr) printf("@%s: %08X\n", #addr, addr);
#define printf_int(addr) printf("%s: %d\n", #addr, addr);
		printf_addr(fifo->write_ptr);
		printf_addr(fifo->buffer);
		printf_addr((char*)fifo->write_ptr - (char*)fifo->buffer);
		printf_int((char*)fifo->write_ptr - (char*)fifo->buffer);
		printf_int(((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T));
		printf_int(((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity);
		printf_int(((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity - 1);
		printf_int((((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity - 1) % fifo->capacity);
		printf_int(((((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity - 1) % fifo->capacity) * sizeof(CFIFO_T));
		printf_addr(((((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity - 1) % fifo->capacity) * sizeof(CFIFO_T) + (char*)fifo->buffer);
#endif

		return *(int*)(((((char*)fifo->write_ptr - (char*)fifo->buffer) / sizeof(CFIFO_T) + fifo->capacity - 1) % fifo->capacity) * sizeof(CFIFO_T) + (char*)fifo->buffer);
	}
	else
	{
		CFIFO_T blank;
		pelib_init(CFIFO_T)(&blank);
		return blank;
	}
 }
*/

/*
void
do_nothing()
{
	printf("Doing nothing\n");
 // Do nothing
}

#define RC_cache_invalidate do_nothing
*/

CFIFO_T
pelib_cfifo_pop(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
  {
    CFIFO_T *ptr,  res;
    //CFIFO_T def;

    ptr = cfifo_peek(CFIFO_T)(fifo, 0);
    if (ptr != NULL)
      {
        fifo->read = (fifo->read + 1) % fifo->capacity;
        fifo->last_op = PELIB_CFIFO_POP;

	res = *ptr;
	/*
	while(res == (CFIFO_T) MARKER)
	{
		
		res = *ptr;
	}
	*ptr = (CFIFO_T) MARKER;
	*/

        return res;
      }
    else
      {
        //return def;
	CFIFO_T def;
	pelib_init(CFIFO_T)(&def);
	return def;
      }
  }

size_t
pelib_cfifo_discard(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, size_t num)
{
	int length = pelib_cfifo_length(CFIFO_T)(*fifo);
	num = num < (size_t) length ? num : (size_t)length;

	fifo->read = (fifo->read + num) % fifo->capacity;

	if(num > 0)
	{
		fifo->last_op = PELIB_CFIFO_POP;
	}

	return num;
}

CFIFO_T
pelib_cfifo_peek(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo, size_t offset)
  {
    CFIFO_T *ptr, res;
    //CFIFO_T def;
    ptr = cfifo_peek(CFIFO_T)(cfifo, offset);
    if (ptr != NULL)
      {
	
	res = *ptr;
	//int cnt = 0;
	/*
	while(res == (CFIFO_T) MARKER)
	{
		debug
		
		res = *ptr;
	}
	*/
	

        return res;
      }
    else
      {
        // return def; //uninitialized??
	CFIFO_T def;
	pelib_init(CFIFO_T)(&def);
	return def;
      }
  }

int
pelib_cfifo_is_full(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
  {
    return cfifo_state(CFIFO_T)(cfifo) == FULL;
  }

int
pelib_cfifo_is_empty(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
  {
    return cfifo_state(CFIFO_T)(cfifo) == EMPTY;
  }

size_t
pelib_cfifo_pushmem(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, CFIFO_T* mem, size_t num)
{
	size_t left, length, pushed;
	//CFIFO_T* end;
	switch(cfifo_state(CFIFO_T)(fifo))
	{
		case EMPTY:
	case NORMAL:			
	  
	  
			// Copy from write to end of buffer
			left = fifo->capacity - fifo->write;
			length = left < num ? left : num;
#if 0

 exit(50);
 if(check_memory(CFIFO_T)(&fifo->buffer[fifo->write],1) && (int)fifo->buffer[fifo->write] != 0)
   {
     //printf("pushmem check failed. value in buffer %x, count: %i \n", (int)fifo->buffer[fifo->write],count);
     exit(77);
   }
#endif

			memcpy((void*)&fifo->buffer[fifo->write], mem, length * sizeof(CFIFO_T));

			// Record amount of elements pushed
			pushed = length;
			// Update fifo's write pointer
      			fifo->write = (fifo->write + length) % fifo->capacity;

			if(pushed > 0)
			{
				fifo->last_op = PELIB_CFIFO_PUSH;
			}
			
			// If left was not enough to accomodate the memory buffer to write
			if(left < num)			
			{
				// Now we are in the reverse mode, just call the function again
				pushed += pelib_cfifo_pushmem(CFIFO_T)(fifo, mem + length, num - length);
			}

			return pushed;
		break;
		
		case FULL:
		case REVERSE:


			// Copy from write to read
			left = fifo->read - fifo->write;
			length = left < num ? left : num;
#if 0

 if(check_memory(CFIFO_T)(&fifo->buffer[fifo->write],1) && (int)fifo->buffer[fifo->write] != 0)
   {
     //printf("pushmem check failed. value in buffer %x, count: %i \n", (int)fifo->buffer[fifo->write],count);
     exit(78);
     }
#endif


			memcpy((void*)&(fifo->buffer[fifo->write]), mem, length * sizeof(CFIFO_T));

			fifo->write += length;
			pushed = length;

			if(pushed > 0)
			{
				fifo->last_op = PELIB_CFIFO_PUSH;
			}

			return pushed;
		break;

		default:
			return 0;
		break;
	}
}

size_t
pelib_cfifo_popmem(CFIFO_T)(cfifo_t(CFIFO_T) *fifo, CFIFO_T* mem, size_t num)
{
	size_t i;
	size_t left, length, popped;
	//CFIFO_T* end;
	volatile int* ptr;
	size_t intlength;

	switch(cfifo_state(CFIFO_T)(fifo))
	{
		case FULL:
		case REVERSE:
			// Copy from write to end of buffer
			left = fifo->capacity - fifo->read;
			length = left < num ? left : num;
/*
debug 
			while(!check_memory(CFIFO_T)((CFIFO_T*)&(fifo->buffer[fifo->read]), length))
			{
debug
				
debug
			}
debug
*/

                        memcpy(mem, (void*)&(fifo->buffer[fifo->read]), length * sizeof(CFIFO_T));
                        
                        intlength = (length * sizeof(CFIFO_T)) / sizeof(int) ;
                        ptr = (volatile int*) (&fifo->buffer[fifo->read]);
			/*
                        for(i = 0; i < intlength; i++)
			  {
			    //pelib_scc_force_wcb();
			    //

			    //printf("ptr is %x, has value %x. buffer is %x, has value %x\n",&ptr[i],ptr[i],&fifo->buffer[fifo->read],fifo->buffer[fifo->read]);
			    //pelib_scc_force_wcb();
			    

			    ptr[i] = MARKER;
			    //

			    //printf("ptr is %x, has value %x. buffer is %x, has value %x\n",&ptr[i],ptr[i],&fifo->buffer[fifo->read],fifo->buffer[fifo->read]);
			    //pelib_scc_force_wcb();
			    //

			  }
			*/

			//pelib_scc_force_wcb();
			
			/*
			if(check_memory(CFIFO_T)((CFIFO_T*)&(fifo->buffer[fifo->read]),1))
			   {
			     printf("Error: MARKER not in the buffer!. value: %x. Supposed to be %x \n",*(int*)(fifo->buffer+fifo->read), (int) MARKER);
			     
			     exit(78);
			   }
			



			if(intlength!=1)
			  {	
			    printf("intlength=%i, length= %i\n",intlength,length);
			    //exit(66);
			    }*/
			//fifo->buffer[fifo->read] = MARKER;

			// Record amount of elements pushed
			popped = length;

			// Update fifo's write pointer
			fifo->read = (fifo->read + length) % fifo->capacity;

			if(popped > 0)
			{
				fifo->last_op = PELIB_CFIFO_POP;
			}
			
			// If left was not enough to accomodate the memory buffer to write
			if(left < num)
			{
				// Now we are in the reverse mode, just call the function again
				popped += pelib_cfifo_popmem(CFIFO_T)(fifo, mem + length, num - length);
			}

			return popped;
		break;
		
		case EMPTY:
		case NORMAL:
			// Copy from write to read
			left = fifo->write - fifo->read;
			length = left < num ? left : num;

/*
debug
			while(!check_memory(CFIFO_T)((CFIFO_T*)&(fifo->buffer[fifo->read]), length))
			{
debug
				
debug
			}
debug
*/

			memcpy(mem, (void*)&(fifo->buffer[fifo->read]), length * sizeof(CFIFO_T));
                        
                        intlength = (length * sizeof(CFIFO_T)) / sizeof(int) ;

                        ptr = (volatile int*) (&fifo->buffer[fifo->read]);
			/*
                        for(i = 0; i < intlength; i++)
			  {
			    ptr[i] = MARKER;
			  }
			*/

			//fifo->buffer[fifo->read] = MARKER;
			//pelib_scc_force_wcb();
			/*
			if(intlength!=1)
			  {
			    printf("intlength=%i, length= %i\n",intlength,length);
			    //exit(67);
			    }*/
			fifo->read += length;
			popped = length;

			if(popped > 0)
			{
				fifo->last_op = PELIB_CFIFO_POP;
			}

			return popped;
		break;

		default:
			return 0;
		break;
	}
}

size_t
pelib_cfifo_peekmem(CFIFO_T)(cfifo_t(CFIFO_T)* fifo, CFIFO_T* mem, size_t num, size_t offset)
{
	size_t left, length;
	cfifo_t(CFIFO_T) copy = *fifo;
	copy.read = (copy.read + offset) % copy.capacity;

	switch(cfifo_state(CFIFO_T)(&copy))
	{
		case FULL:
		case REVERSE:
			// Copy from write to end of buffer
			left = copy.capacity - copy.read;
			length = left < num ? left : num;

			memcpy(mem, (void*)&(copy.buffer[copy.read]), length * sizeof(CFIFO_T));

			// If left was not enough to accomodate the memory buffer to write
			if((left) < num && left > 0)
			{
				// Now we are in the reverse mode, just call the function again
				length += pelib_cfifo_peekmem(CFIFO_T)(&copy, mem + length, num - length, length);
			}

			return length;
		break;
		
		case EMPTY:
		case NORMAL:
			// Copy from write to read
			left = copy.write - copy.read;
			length = left < num ? left : num;

			memcpy(mem, (void*)&(copy.buffer[copy.read]), length * sizeof(CFIFO_T));

			return length;
		break;

		default:
			return 0;
		break;
	}
}

#define cfifo_continuous_read_length(elem) PELIB_CONCAT_3(cfifo_, cfifo(elem), _continuous_read_length)
static
size_t
cfifo_continuous_read_length(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
{
	switch(cfifo_state(CFIFO_T)(fifo))
	{
		case FULL:
		case REVERSE:
			return fifo->capacity - fifo->read;
		break;
		
		case EMPTY:
		case NORMAL:
			return fifo->write - fifo->read;
		break;

		default:
			return -1;
		break;
	}
}

#define cfifo_continuous_write_length(elem) PELIB_CONCAT_3(cfifo_, cfifo(elem), _continuous_write_length)
static
size_t
cfifo_continuous_write_length(CFIFO_T)(cfifo_t(CFIFO_T)* fifo)
{
	switch(cfifo_state(CFIFO_T)(fifo))
	{
		case FULL:
		case REVERSE:
			return fifo->read - fifo->write;
		break;
		
		case EMPTY:
		case NORMAL:
			return fifo->capacity - fifo->write;
		break;

		default:
			return -1;
		break;
	}
}

size_t
pelib_cfifo_popfifo(CFIFO_T)(cfifo_t(CFIFO_T)* src, cfifo_t(CFIFO_T)* tgt, size_t num)
{
debug
	size_t requested = num;
debug

	while(num > 0 && !pelib_cfifo_is_empty(CFIFO_T)(src) && !pelib_cfifo_is_full(CFIFO_T)(tgt))
	{
debug
		size_t readl = cfifo_continuous_read_length(CFIFO_T)(src);
debug
		volatile size_t writel = cfifo_continuous_write_length(CFIFO_T)(tgt);
debug
		size_t length = readl < writel ? readl : writel;
debug
		length = num < length ? num : length;
debug

//		printf("readl = %d, writel = %d, num = %d, length = %d, src_empty = %d, tgt_full = %d\n", readl, writel, num, length, pelib_cfifo_is_empty(CFIFO_T)(src), pelib_cfifo_is_full(CFIFO_T)(tgt));
/*
		while(!check_memory(CFIFO_T)((CFIFO_T*)&(src->buffer[src->read]), length))
		{
debug
			
debug
		}
*/
debug
		memcpy((void*)&(tgt->buffer[tgt->write]), (void*)&(src->buffer[src->read]), length * sizeof(CFIFO_T));
debug
		num -= length;
debug

		pelib_cfifo_discard(CFIFO_T)(src, length);
debug
		pelib_cfifo_fill(CFIFO_T)(tgt, length);
debug
	}
debug

	return requested - num;
}

size_t
pelib_cfifo_length(CFIFO_T)(cfifo_t(CFIFO_T) cfifo)
{
	switch(cfifo_state(CFIFO_T)(&cfifo))
	{
		case EMPTY:
			return 0;
		break;
		case NORMAL:
			return cfifo.write - cfifo.read;
		break;
		case REVERSE:
			return cfifo.capacity - (cfifo.read - cfifo.write);
		break;
		case FULL:
			return cfifo.capacity;
		break;
	}

	return 0;
}

CFIFO_T
pelib_cfifo_last(CFIFO_T)(cfifo_t(CFIFO_T) *cfifo)
{	
	size_t length = pelib_cfifo_length(CFIFO_T)(*cfifo);

	if(pelib_cfifo_length(CFIFO_T)(*cfifo) <= 0)
	{  
		CFIFO_T def;
		pelib_init(CFIFO_T)(&def);
		return def;
	}
	else
	{
		CFIFO_T* ptr = cfifo_peek(CFIFO_T)(cfifo, length - 1);
		return *ptr;
	}
}

size_t
pelib_cfifo_capacity(CFIFO_T)(cfifo_t(CFIFO_T)* cfifo)
{
	return cfifo->capacity;
}

