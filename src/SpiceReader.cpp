/**
 * Implements functions for parsing CSV files
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include "../include/SpiceReader.h"

table_t *SpiceReader::getSpiceTable() const {
    return spiceTable;
}

void SpiceReader::read() {
    FILE *file_pointer = fopen(filename, "r");
    if (!file_pointer) {
        fprintf(stderr, "Can't open %s (tried \"r\" mode).\n", filename);
        exit(EXIT_FAILURE);
    }

    //calculate file size
    fseek(file_pointer, 0, SEEK_END);
    auto size = (size_t) ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    //copy content to memory
    auto *file_content = (char *) calloc(size + 1, 1); //+1 for \0
    fread(file_content, size, 1, file_pointer);
    fclose(file_pointer);

    to_upper_case(file_content);

    //before we start tokenizing the content, we save a pointer to the start of the string, so we can free it later
    char *file_content_start = file_content;

    //clear spice table to store file content
    spiceTable = new_table(nullptr);

    //get lines and fill a row out of each
    char *line;
    while ((line = strsep(&file_content, "\n")) && line[0] != -1 /*EOF*/) {

        if (line[0] != '\0') {
            row_t *row = new_row(nullptr);

            //each element between commas turns into a cell, appended to the row
            char *element;
            while ((element = strsep(&line, " \t\r\v\f"))) {
                if (strcmp(element, "\0") != 0) {
                    cell_t *cell = new_cell(new_cell_data(element));
                    append_cell(row, cell);
                }
            }
            //append filled row to table
            append_row(spiceTable, row);
        }
    }
    //free bytes read from file
    free(file_content_start);
}

SpiceReader::SpiceReader(char *filename) : filename(filename) {
    spiceTable = nullptr;
}

SpiceReader::~SpiceReader() {
    clear_table(spiceTable);
    free(spiceTable);
}

void SpiceReader::to_upper_case(char *string) {
    while (*string) {
        *string = (char) toupper((unsigned char) *string);
        ++string;
    }
}
