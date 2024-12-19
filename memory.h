/*
 *     memory.h
 *     by kcasey06 & bdioni01, 1/31/2024
 *     filesofpix
 *
 *     Header file for the memory allocating and freeing portion of the
 *     program. Includes function declarations for functions that allocate
 *     memory for a line, free the memory associated with a given line, free
 *     the memory associated with the program's Hanson structures, and free
 *     table contents to be used in a mapping function. Includes standard
 *     libraries.
 *     
 */

#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "table.h"
#include "list.h"

char *malloc_line(size_t size);
void free_line(char *line);
void free_line_apply(const void *key, void **value, void *closure);
void structures_free(Table_T *my_table, List_T *my_list);

#endif