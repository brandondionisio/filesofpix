/*
 *     readaline.c
 *     by kcasey06 & bdioni01, 1/30/2024
 *     filesofpix
 *
 *     Function implementations for the readaline program.
 *     Reads in a line in the given filestream until an endline character or
 *     EOF. Expands the allocated memory if needed to allow for processing of a
 *     theoretical infinitely long line. Sets datapp to point to the address of
 *     first character of the line and sets inputfd to point to the first
 *     character of the next line and NULL if there are no more lines to read.
 *     Returns the size of the line just read in.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

size_t readaline(FILE *inputfd, char **datapp);
char *expand(char *line, size_t *size);
void read_through(int *c_int,
                  size_t *counter,
                  size_t init_cap,
                  char **line_p,
                  FILE *inputfd);
int get_char (FILE *inputfd, size_t *counter) ;

/*************readaline**************
 * Use:
 *      Reads in a line in the given filestream until an endline character
 *      or EOF. Sets datapp to point to the address of the first char of
 *      that line and inputfd to point to the next line in the stream (if not
 *      EOF). Returns the size of the line that was read in
 * Return:
 *      size_t variable representing the length of the line that was read in
 * Parameters:
 *      FILE *inputfd:         pointer to a input file stream
 *      char **datapp:         char pointer that points to the address of the
 *                             first byte of the line we are reading
 * Expects:
 *      inputfd points to a filestream that is not NULL or EOF
 *      file can be read in correctly without fail
 *      each line is terminated with an endline character
 */
size_t readaline(FILE *inputfd, char **datapp)
{
        /* Checked runtime error for NULL arguments */
        assert(inputfd != NULL && datapp != NULL);

        /* malloc memory and check for allocation failure */
        const size_t initial_capacity = 1000;
        char *line = malloc(initial_capacity * sizeof(char));
        assert(line != NULL);
        
        size_t counter = 0;
        
        /* read in chars from file until error or EOF */
        int c_int = get_char(inputfd, &counter);
        read_through(&c_int, &counter, initial_capacity, &line, inputfd);

        if (c_int == -1) {
                assert(!ferror(inputfd)); /* CRE for error reading */
                assert(feof(inputfd)); /* CRE to confirm EOF */
                
                *datapp = NULL;
                free(line);
                return 0;
        } else {
                *datapp = line; 
        }

        line[counter - 1] = '\n'; /* add newline character to end of line */
        return counter;
}

/*************expand**************
 * Use:
 *      expand the memory allocated for reading in the line such that
 *      we can read in lines of infinite lengths (doubles the size).
 * Return:
 *      char pointer set to the beginning of the line that is being read in
 * Parameters:
 *      char *line:            char pointer to the beginning of the c-string
 *                             which is reading in the current line
 *      size_t *size:          current size of the malloc'd c-string
 * Expects:
 *      size represents the current size of line
 */
char *expand(char *line, size_t *size)
{
        *size *= 2;
        line = realloc(line, *size * sizeof(char));
        assert(line != NULL);
        return line;
}

/*************read_through**************
 * Use:
 *      helper function that reads through the current line in the input file
 *      until c_int points to a newline character or EOF.
 * Return:
 *      None
 * Parameters:
 *      int *c_int:            pointer to the first char in the current line of
 *                             input
 *      size_t *counter:       size_t pointer to the number of chars we are
 *                             reading in from the current line
 *      size_t init_cap:       size_t representing the initial capacity of
 *                             chars we can read in from the input stream
 *      char **line_p:         pointer to the address of the beginning of the
 *                             c-string where we are storing the line
 *      FILE *inputfd:         pointer to the beginning of the line in the
 *                             input stream
 * Expects:
 *      None
 */
void read_through(int *c_int,
                  size_t *counter,
                  size_t init_cap,
                  char **line_p,
                  FILE *inputfd)
{
        /* read through line until newline or error/EOF */
        while ((char)(*c_int) != '\n' && (*c_int) != -1) {

                if (*counter == init_cap - 1) {
                    *line_p = expand(*line_p, &init_cap); /* expand capacity */
                }

                /* add char value of retrieved value to line */
                (*line_p)[(*counter) - 1] = (char)(*c_int);
                
                /* read in char from file and check for error */
                *c_int = get_char(inputfd, counter);
        }
}

/*************get_char**************
 * Use:
 *      read in the next char from the given input file stream
 *      check for errors when reading
 * Return:
 *      next char in the input stream
 * Parameters:
 *      FILE *inputfd:         pointer to the current char in the input file
 *      size_t *counter:       size_t pointer that tracks the length of the
 *                             line being read in
 * Expects:
 *      no errors when reading in from the input stream
 */
int get_char(FILE *inputfd, size_t *counter) 
{
    int c_int = fgetc(inputfd);
    assert(!ferror(inputfd));
    (*counter)++;

    return c_int;
}