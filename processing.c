/*
 *     processing.c
 *     by kcasey06 & bdioni01, 1/30/2024
 *     filesofpix
 *
 *     Function implementations for the processing program.
 *     Includes functions and helper functions that convert and process various
 *     c-strings todifferent formats, such as getting an infusion sequence from 
 *     a plain line and converting from plain to raw pgm format. 
 */


#include "processing.h"

/**************plain_to_infusion*****************
 * Use:
 *      with the given c-string, return the infusion sequence of that line.
 *      set size to equal the length of the infusion sequence.
 * Return:
 *      char pointer to an infusion sequence
 * Parameters:
 *      char *line:            plain line to extract the infusion sequence
 *      int *size:             holds the infusion sequence size outside the
 *                             scope of the function
 *      size_t num:            size necessary to malloc container for infusion
 *                             sequence
 * Expects:
 *      num to be as big as the memory in line
 * Notes:
 *      May CRE if malloc fails
 */
char *plain_to_infusion(char *line, int *size, size_t num)
{
        char *infusion = malloc_line(num);

        int infusion_counter = 0; /* counter for size of infusion seq */
        int counter = 0; /* counter to cycle through line */

        /* cycle through line, adding non-digit chars to infusion sequence */
        while (line[counter] != '\n') {
                if (!isdigit(line[counter])) {
                    infusion[infusion_counter] = line[counter];
                    infusion_counter++;
                }
                counter++;
        }

        *size = infusion_counter;

        return infusion;
}

/*************plain_to_raw**************
 * Use:
 *      with the given c-string, return the raw version of that line.
 *      set width to equal the length of the raw line.
 * Return:
 *      char pointer to a raw line
 * Parameters:
 *      char *line:            plain line to extract the raw line
 *      size_t num:            size necessary to malloc container for the raw
 *                             line
 *      int *width:            holds the length of the raw line (width of the
 *                             pgm file) outside the scope of the function
 * Expects:
 *      num to be as big as the memory in line
 * Notes:
 *      May CRE if malloc fails
 */
char *plain_to_raw(char *line, size_t num, int *width)
{
        char *new_line = malloc(num);
        
        char *remaining = line;
        int raw_counter = 0;

        while (*remaining != '\n') {
                /* skip non-digit chars, returning new_line if at end of line */
                find_digit(&remaining);
                if (*remaining == '\n') {
                        return new_line;
                }

                /* get next sequence of digits from the line */
                int digits = get_digits(&remaining);
            
                /* store the converted integer as a character in new_line */
                new_line[raw_counter] = (char)digits;
                raw_counter++;
                *width = raw_counter;
        }
        return new_line;
}


/*************get_digits**************
 * Use:
 *      get all the digits from where remaining_p points to until the first
 *      non-digit character (ex. 123abc -->  123 & remaining_p->abc)
 * Return:
 *      the digits that remaining_p points to
 * Parameters:
 *      char **remaining_p:    char pointer that points to the address of the
 *                             current char of the plain line we are reading
 * Expects:
 *      remaining_p points to a digit
 * Notes: 
 *      May CRE if strtol fails
 */
int get_digits(char **remaining_p) 
{
        /* convert digits to an integer */
        char *temp = *remaining_p;
        int digits = (int)strtol(*remaining_p, &temp, 10);

        /* checks for strtol failure if temp hasn't been changed */
        assert(temp != *remaining_p);

        /* adjust remaining to first non-digit character */
        *remaining_p = temp;

        return digits;
}

/******************find_digit*****************
 * Use:
 *      Move pointer through provided line until a digit is found
 * Return:
 *     None
 * Parameters:
 *      char **remaining:   address of char-pointer to line of chars to be
 *                          parsed through 
 * Expects:
 *      remaining_p should not be NULL and there should be a Newline character
 *      at the end of the provided line
 */
void find_digit(char **remaining_p) 
{
        /* cycle through remaining line until a digit is reached */
        while (!isdigit(*(*remaining_p))) {
                (*remaining_p)++;
                    
                /* if new line character is found, stop reading the line */
                if (*(*remaining_p) == '\n') {
                        return;
                }
        }
}