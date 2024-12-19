/*
 *     restoration.c
 *     by kcasey06 & bdioni01, 1/30/2024
 *     filesofpix
 *
 *     Function implementations for the restoration program.
 *     Handles arguments, reads lines in from the file, stores lines
 *     in a Hanson Table, stores repeated infusion sequence lines into a
 *     Hanson list, and prints out the raw content of the file for image
 *     processing.
 */

#include "readaline.h"
#include "restoration.h"
#include "processing.h"
#include "memory.h"

/******************main***************
 * Use:
 *      main driver of the program, handles arguments, determines program's
 *      success.
 * Return:
 *      0 if executed to completion -- non-zero integer otherwise.
 * Parameters:
 *      int *argc:             number of arguments in program call
 *      char *argv[]:          pointer to an array of arguments
 * Expects:
 *      two arguments or less
 *      file that exists
 * Notes:
 *      Will CRE if more than 2 arguments are provided
 */
int main(int argc, char *argv[])
{
        assert(argc <= 2);
        
        if (argc == 2) {

                FILE *fp = file_open(argv[1]);

                restoration(fp);

                fclose(fp); /* close file */
            
        } else {
                restoration(stdin);
        }

        return EXIT_SUCCESS;
}

/*************restoration**************
 * Use:
 *      Restores an image to raw pgm format from the given corrupted plain file
 * Return:
 *      None
 * Parameters:
 *      File *fp:              pointer to a file stream to read from
 * Expects:
 *      Valid file stream fp
 */
void restoration(FILE *fp)
{
        char *line;
        int width = 0;
        Table_T my_table = Table_new(1000, NULL, NULL);
        List_T my_list = List_list(NULL);

        size_t num = readaline(fp, &line);
        while (line != NULL) {
                const char *inf_atom = get_atom(line, num);
                char *original_repeat = Table_put(my_table, inf_atom, line);

                /*see if infusion sequence has been found with duplicate atom*/
                if (add_duplicates(original_repeat, &num, &width, &my_table,
                                   &my_list, inf_atom)) {

                        num = readaline(fp, &line);
                        
                        /* loop through lines, adding originals to list */
                        add_list(&line, &num, &my_list, inf_atom, fp, &width);
                        
                        free_line(line); 

                        print_image(&my_list, width);
                }
                num = readaline(fp, &line);
        }
        structures_free(&my_table, &my_list);
}

/*************file_open**************
 * Use:
 *      Attempts to open and read the given file -- throws a checked runtime
 *      error if fails
 * Return:
 *      pointer to a file stream
 * Parameters:
 *      const char *filename:  const char pointer to a c-string of a filename
 * Expects:
 *      filename to be a correct file name
 *      filename be a readable file type
 * Notes:
 *      Will CRE if file fails to open
 */
FILE *file_open(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    /* CRE if file opening fails */
    assert(fp != NULL);
    return fp;

}


/******************get_atom*****************
 * Use:
 *      Create atom corresponding to infusion sequence of a line
 * Return:
 *      Atom of infusion sequence
 * Parameters:
 *      char *line:            pointer to first char of line read from file
 *      size_t num:            size in bytes of line     
 * Expects:
 *      line must not be null
 */
const char *get_atom(char *line, size_t num) 
{
        int atom_size = 0; 
        
        /* get infusion (non-digit) sequence from line and create corr. atom */
        char *infusion = plain_to_infusion(line, &atom_size, num);
        const char *infusion_atom = Atom_new(infusion, atom_size);

        free_line(infusion);
        return infusion_atom;
}

/******************add_duplicates*****************
 * Use:
 *      If a repeated infusion sequence is found, adds the first two
 *      repeated plain lines from the table into the list
 * Return:
 *      boolean representing whether or not a repeat was found
 * Parameters:
 *      char *original repeat:     pointer to the first char of the plain
 *                                 line that was potentially repeated
 *      size_t *num:               size_t pointer that holds the size necessary
 *                                 to malloc container for the raw line
 *      int *width:                int pointer that is set to the width of
 *                                 the raw lengths of the lines
 *      Table_T *my_table:         pointer to the first element of a Hanson
 *                                 Table
 *      List_T *my_list:           pointer to the first element of a Hanson
 *                                 List
 *      const char *infusion atom: const char pointer to an atom containing a
 *                                 c-string infusion sequence.
 * Expects:
 *      original_repeat is not null for the duplicate function to work
 */
bool add_duplicates(char *original_repeat,
                    size_t *num,
                    int *width,
                    Table_T *my_table,
                    List_T *my_list,
                    const char *infusion_atom)
{
        /* check if insertion into table produced a value of duplicate key */
        if (original_repeat != NULL) {

                /* add original raw duplicate to front of the list */
                char *original_raw = plain_to_raw(original_repeat, *num, width);
                *my_list = List_push(*my_list, original_raw);
                free_line(original_repeat);

                /* add second raw to front of the list */
                char *second_repeat = Table_get(*my_table, infusion_atom);
                char *second_raw = plain_to_raw(second_repeat, *num, width);
                *my_list = List_push(*my_list, second_raw);

                return true;
        }
        return false; /* false when there was no duplicate key in table */
}

/******************add_list*****************
 * Use:
 *      Finds every original corrupted line and adds the raw, filtered version
 *      to the list or original lines 
 * Return:
 *      None
 * Parameters:
 *      char **line:                address of char pointer to line being read
 *                                  from file
 *      size_t *num:                size_t pointer that holds the size necessary
 *                                  to malloc container for the raw line and 
 *                                  atom
 *      List_T *my_list:            pointer to the first element of a Hanson
 *                                  List
 *      const char *infusion atom:  const char pointer to an atom containing a
 *                                  c-string infusion sequence
 *      File *fp:                   pointer to a file stream to read from
 *      int *width:                 int pointer that is set to the width of the
 *                                  raw lengths of the lines
 * Expects:
 *      line must not be null, fp must be a valid file stream
 */
void add_list(char **line,
              size_t *num,
              List_T *my_list,
              const char *infusion_atom,
              FILE *fp,
              int *width) 
{
        /* loop through rest of the file, adding to list if original*/
        while (*line != NULL) {

                    const char *new_infusion_atom = get_atom(*line, *num);

                    /* check if line is original through same infusino seq */
                    if (new_infusion_atom == infusion_atom) {
                            /* add raw original line to list */
                            char *repeat_raw = plain_to_raw(*line, *num, width);
                            *my_list = List_push(*my_list, repeat_raw);
                    }

            free_line(*line);
            *num = readaline(fp, line);
        }
}

/*************print_image**************
 * Use:
 *      prints the contents of the list and image info such that
 *      we can process it into an actual image
 * Return:
 *      None
 * Parameters:
 *      List_T *my_list:       pointer to the first element of a Hanson List
 *      int width:             int that stores the width of the image
 * Expects:
 *      None
 */
void print_image(List_T *my_list, int width) 
{
        const int height = List_length(*my_list);
        const int MAXVAL = 255;

        /* print header of raw file */
        printf("P5\n%d %d\n%d\n", width, height, MAXVAL);

        /* reverse list so that first original line is at front of list */
        *my_list = List_reverse(*my_list);

        /* var to hold value popped from front of list - line to be read */
        char *popped_line = NULL;

        /* pop lines off of list until it is empty */
        while (List_length(*my_list) != 0) {
                    
                *my_list = List_pop(*my_list, (void**)&popped_line);

                /* print every character from the line */
                for (int i = 0; i < width; i++) {
                    printf("%c", popped_line[i]);
                }
                free_line(popped_line);
        }
}