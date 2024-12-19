/*
 *     restoration.h
 *     by kcasey06 & bdioni01, 1/30/2024
 *     filesofpix
 *
 *     Header file for the restoration program. Includes function declarations
 *     for file processing, raw restoration, and helper functions regarding
 *     Hanson Lists and Tables. Includes standard libraries, Hanson structure 
 *     libraries, and other header files necessary for the program to run.
 */

#ifndef RESTORATION_H
#define RESTORATION_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "atom.h"
#include "table.h"
#include "list.h"
#include "readaline.h"
#include "processing.h"
#include "memory.h"

void restoration(FILE *fp);
FILE *file_open(const char *filename);
void print_image(List_T *my_list, int width);
const char *get_atom(char *line, size_t num);
bool add_duplicates(char *original_repeat,
                    size_t *num,
                    int *width,
                    Table_T *my_table,
                    List_T *my_list,
                    const char *infusion_atom);
void add_list(char **line,
              size_t *num,
              List_T *my_list,
              const char *infusion_atom,
              FILE *fp,
              int *width);

#endif