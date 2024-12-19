/*
 *     memory.c
 *     by kcasey06 & bdioni01, 1/31/2024
 *     filesofpix
 *
 *     Function implementations for the memory freeing portion of the program.
 *     Allocates memory for a line with a given size, frees the memory
 *     associated with a given line, frees the memory associated with the
 *     program's Hanson structures, and frees the table contents to be used in
 *     a mapping function.
 */

#include "memory.h"

char *malloc_line(size_t size)
{
    char *line = malloc(size);
    assert(line != NULL);
    return line;
}

/*************free_line**************
 * Use:
 *      frees the memory associated with the given c-string line
 * Return:
 *      None
 * Parameters:
 *      char *line:            pointer to line to be freed
 * Expects:
 *      None
 * Notes:
 *      None
 */
void free_line(char *line)
{
        /* check if line is null to prevent double free */
        if (line != NULL) {
            free(line);
            line = NULL;
            assert(line == NULL);
        }
}

/*************free_line_apply************
 * Use:
 *      Mapping function for a hanson table which frees the dynamically
 *      allocated memory.
 * Return:
 *      None
 * Parameters:
 *      const void *key:       pointer to a key in a Hanson Table
 *      void **value:          pointer to a value in a Hanson Table
 *      void *closure:         pointer to a closure for a Hanson Table
 * Expects:
 *      None
 */
void free_line_apply(const void *key, void **value, void *closure)
{
        (void)key;
        (void)closure;
        free_line(*value);
}

/*************structures_free**************
 * Use:
 *      Frees the dynamic memory associated with the given table and list
 * Return:
 *      None
 * Parameters:
 *      Table_T *my_table:     pointer to a Hanson Table
 *      List_T *my_list:       pointer to a Hanson List
 * Expects:
 *      --
 */
void structures_free(Table_T *my_table, List_T *my_list) 
{
        Table_map(*my_table, free_line_apply, NULL); 
        Table_free(my_table);
        List_free(my_list);
}