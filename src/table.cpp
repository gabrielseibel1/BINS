/**
 * Implementations for table.h
 * Implements functions for tables, made of rows (which are made of cells). Each cell of the table contains data.
 * Implements constructors, insertions, accesses (getter), printing and clearing for the data structures.
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "../include/table.h"

data_t* new_cell_data(char* raw_data) {
    auto * typed_data = (data_t*) calloc(sizeof(data_t), 1);

    //use strtod() to see if raw_data is purely a double, or just an alphanumerical string

    /* QUOTE FROM strtod MANUAL: double strtod(const char *nptr, char **endptr);
     * If endptr is not nullptr, a pointer to the character after the last  character
     * used  in  the conversion is stored in the location referenced by endptr.
     * If no conversion is performed, zero is returned and the value  of  nptr
     * is stored in the location referenced by endptr.
     */

    char* end_pt;
    double double_data = strtod(raw_data, &end_pt);

    if ((end_pt != nullptr && *end_pt != '\0') // raw_data contained something else after the double
        || (double_data == 0 && strlen(raw_data) == strlen(end_pt)) ) //no conversion performed
    {
        typed_data->type = CELL_DATA_TYPE_STRING;
        typed_data->value._string = strdup(raw_data);

    } else {                                // conversion was performed successfully
        typed_data->type= CELL_DATA_TYPE_DOUBLE;
        typed_data->value._double = double_data;
    }

    return typed_data;
}

cell_t* new_cell(data_t* data) {
    auto *cell = (cell_t *) malloc(sizeof(cell_t));
    cell->data = data;
    cell->next_cell = nullptr;
    return cell;
}

row_t *new_row(cell_t *first_cell) {
    auto *row = (row_t *) malloc(sizeof(row_t));
    row->cells = first_cell;
    row->index = 0;
    row->next_row = nullptr;
    return row;
}

table_t *new_table(row_t *first_row) {
    auto *table = (table_t *) malloc(sizeof(table_t));
    table->rows = first_row;
    return table;
}

void append_cell(row_t *row, cell_t *cell_to_append) {
    if (!row) {
        fprintf(stderr, "Row is nullptr, can't append cell.\n");
        exit(EXIT_FAILURE);
    }

    //if there are no cells, append as first one
    if (!row->cells) {
        row->cells = cell_to_append;
        return;
    }

    //look for last cell of the row
    cell_t *current_cell = row->cells;
    while (current_cell->next_cell) {
        current_cell = current_cell->next_cell;
    }
    //here, current_cell is the last cell, so we append to it
    current_cell->next_cell = cell_to_append;
}

void append_row(table_t *table, row_t *row_to_append) {
    if (!table) {
        fprintf(stderr, "Table is nullptr, can't append row.\n");
        exit(EXIT_FAILURE);
    }
    
    //if there are no rows, append as first one
    if (table->rows == nullptr) {
        table->rows = row_to_append;
        return;
    }

    //look for last row of the table
    row_t *current_row = table->rows;
    while (current_row->next_row) {
        current_row = current_row->next_row;
    }
    //here, current_row is the last row, so we append to it
    current_row->next_row = row_to_append;
    row_to_append->index = current_row->index + 1;
}

void print_cell(cell_t *cell) {
    if (!cell) {
        fprintf(stderr, "Cell is nullptr, can't print it.\n");
        exit(EXIT_FAILURE);
    }
    print_cell_data(cell->data);
}

void print_cell_data(const data_t *cell_data) {
    if (cell_data->type == CELL_DATA_TYPE_STRING) {
        printf("\"%s\" ", cell_data->value._string);
    } else if (cell_data->type == CELL_DATA_TYPE_DOUBLE) {
        printf("%e ", cell_data->value._double);
    } else {
        printf("? (cell of no type)");
    }
}

void print_row(row_t *row) {
    printf("#%d: %s { ", row->index, row_type_to_string(row->type));
    cell_t *cell = row->cells;
    while (cell) {
        print_cell(cell);
        cell = cell->next_cell;
    }
    printf("}\n");
}

void print_table(table_t *table) {
    if (!table) {
        fprintf(stderr, "Table is nullptr, can't print it.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\nLINES OF SPICE FILE:\n");

    row_t *row = table->rows;
    while (row) {
        print_row(row);
        row = row->next_row;
    }
}

void clear_cell(cell_t* cell) {
    if (cell->data->type == CELL_DATA_TYPE_STRING) free(cell->data->value._string);
    free(cell->data);
}

void clear_row(row_t* row) {
    if (!row) {
        fprintf(stderr, "Row is nullptr, can't clear it.\n");
        exit(EXIT_FAILURE);
    }

    cell_t* cell = row->cells;
    while (cell) {
        clear_cell(cell);
        cell_t* next_cell = cell->next_cell;
        free(cell);
        cell = next_cell;
    }
}

void clear_table(table_t* table) {
    if (!table) {
        fprintf(stderr, "Table is nullptr, can't clear it.\n");
        exit(EXIT_FAILURE);
    }

    row_t* row = table->rows;
    while (row) {
        clear_row(row);
        row_t* next_row = row->next_row;
        free(row);
        row = next_row;
    }
}

cell_t* get_cell(table_t* table, int row_index, int cell_index) {
    if (!table) {
        fprintf(stderr, "Table is nullptr, can't get cell [%d][%d].\n", row_index, cell_index);
        exit(EXIT_FAILURE);
    }

    //find row
    row_t* row = table->rows;
    for (int i = 0; i < row_index; ++i) {
        if (!row) return nullptr;
        row = row->next_row;
    }
    if (!row) return nullptr;

    //find cell
    cell_t* cell = row->cells;
    for (int j = 0; j < cell_index; ++j) {
        if (!cell) return nullptr;
        cell = cell->next_cell;
    }
    if (!cell) return nullptr;

    return cell;
}

int table_test() {
    table_t *table = new_table(nullptr);
    for (int i = 0; i < 5; ++i) {
        row_t* row = new_row(nullptr);
        for (int j = 0; j < 3; ++j) {
            char string[2];
            sprintf(string, "%d", j);
            append_cell(row, new_cell(new_cell_data(string)));
        }
        append_row(table, row);
    }

    printf("Testing print_table(table):\n");
    print_table(table);
    printf("\n");

    printf("Testing get_cell(cell, i, j):\n");
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            cell_t* cell = get_cell(table, i, j);
            if (cell) print_cell(cell);
        }
        printf("\n");
    }

    clear_table(table);
    free(table);

    return 0;
}

const char *row_type_to_string(RowType type) {
    switch (type) {
        case CMT:   return "COMMENT  ";
        case CMD:   return "COMMAND  ";
        case C:     return "CAPACITOR";
        case D:     return "DIODE    ";
        case E:     return "VCVS     ";
        case F:     return "CCCS     ";
        case G:     return "VCCS     ";
        case H:     return "CCVS     ";
        case I:     return "I-SOURCE ";
        case L:     return "INDUCTOR ";
        case M:     return "MOSFET   ";
        case Q:     return "BJT      ";
        case R:     return "RESISTOR ";
        case V:     return "V-SOURCE ";
        default:break;
    }
}