/*
 *     processing.h
 *     by kcasey06 & bdioni01, 1/31/2024
 *     filesofpix
 *
 *     Header file for the processing portion of the program. Includes
 *     function declarations for functions that extract the infusion sequence
 *     from a plain line, convert a plain line to a raw line, get the next
 *     contiguous digits from a pointer to a c-string, and find the next
 *     digit character from a pointer to a c-string. Includes standard
 *     libraries.
 *     
 */

#ifndef PROCESSING_H
#define PROCESSING_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "memory.h"

char *plain_to_infusion(char *line, int *size, size_t num);
char *plain_to_raw(char *line, size_t num, int *width);
int get_digits(char **remaining_p);
void find_digit(char **remaining_p);

#endif