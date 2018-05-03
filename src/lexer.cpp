/**
 * Implements functions for parsing CSV files
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include "../include/lexer.h"

table_t* lex(char *filename) {
    FILE* file_pointer = fopen(filename, "r");
    if (!file_pointer) {
        fprintf(stderr, "Can't open %s (tried \"r\" mode).\n", filename);
        exit(EXIT_FAILURE);
    }

    //calculate file size
    fseek(file_pointer, 0, SEEK_END);
    size_t size = (size_t) ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    //copy content to memory
    char* file_content = (char *) calloc(size + 1, 1); //+1 for \0
    fread(file_content, size, 1, file_pointer);
    fclose(file_pointer);

    to_upper_case(file_content);

    //before we start tokenizing the content, we save a pointer to the start of the string, so we can free it later
    char* file_content_start = file_content;

    //create a table to store file content
    table_t* table = new_table(NULL);

    //get lines and fill a row out of each
    char* line;
    while ((line = strsep(&file_content, "\n")) && line[0] != -1 /*EOF*/) {

        if (line[0] != '\0') {
            row_t* row = new_row(NULL);

            //each element between commas turns into a cell, appended to the row
            char* element;
            while ((element = strsep(&line, " \t\r\v\f"))) {
                if (strcmp(element, "\0") != 0) {
                    cell_t* cell = new_cell(new_cell_data(element));
                    append_cell(row, cell);
                }
            }
            //append filled row to table
            append_row(table, row);
        }
    }
    //free bytes read from file
    free(file_content_start);

    return table;
}

void to_upper_case(char* string) {
    while (*string) {
        *string = (char) toupper((unsigned char) *string);
        ++string;
    }
}

void lex_test() {
    printf("data_test.csv:\n");
    table_t* table_test = lex("../data/data_test.csv");
    print_table(table_test);
    clear_table(table_test);
    free(table_test);

    printf("netlist.spc:\n");
    table_t* table_spc = lex("../data/netlist.spc");
    print_table(table_spc);
    clear_table(table_spc);
    free(table_spc);
}
