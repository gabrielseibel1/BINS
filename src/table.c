/**
 * Implementations for table.h
 * Implements functions for tables, made of rows (which are made of cells). Each cell of the table contains data.
 * Implements constructors, insertions, accesses (getter), printing and clearing for the data structures.
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "../include/table.h"

t_cell_data* new_cell_data(char* raw_data) {
    t_cell_data* typed_data = (t_cell_data*) calloc(sizeof(t_cell_data), 1);

    //use strtof() to see if raw_data is purely a float, or just an alphanumerical string

    /* QUOTE FROM STRTOF MANUAL: float strtof(const char *nptr, char **endptr);
     * If endptr is not NULL, a pointer to the character after the last  character
     * used  in  the conversion is stored in the location referenced by endptr.
     * If no conversion is performed, zero is returned and the value  of  nptr
     * is stored in the location referenced by endptr.
     */

    char* end_pt;
    float float_data = strtof(raw_data, &end_pt);

    if ((end_pt != NULL && *end_pt != '\0') // raw_data contained something else after the float
        || (float_data == 0 && strlen(raw_data) == strlen(end_pt)) ) //no conversion performed
    {
        typed_data->type = CELL_DATA_TYPE_STRING;
        typed_data->value._string = strdup(raw_data);

    } else {                                // conversion was performed successfully
        typed_data->type= CELL_DATA_TYPE_FLOAT;
        typed_data->value._float = float_data;
    }

    return typed_data;
}

t_cell* new_cell(t_cell_data* data) {
    t_cell *cell = (t_cell *) malloc(sizeof(t_cell));
    cell->data = data;
    cell->next_cell = NULL;
    return cell;
}

t_row *new_row(t_cell *first_cell) {
    t_row *row = (t_row *) malloc(sizeof(t_row));
    row->cells = first_cell;
    row->next_row = NULL;
    return row;
}

t_table *new_table(t_row *first_row) {
    t_table *table = (t_table *) malloc(sizeof(t_table));
    table->rows = first_row;
    return table;
}

void append_cell(t_row *row, t_cell *cell_to_append) {
    if (!row) {
        printf("Row is NULL, can't append cell.\n");
        exit(EXIT_FAILURE);
    }

    //if there are no cells, append as first one
    if (!row->cells) {
        row->cells = cell_to_append;
        return;
    }

    //look for last cell of the row
    t_cell *current_cell = row->cells;
    while (current_cell->next_cell) {
        current_cell = current_cell->next_cell;
    }
    //here, current_cell is the last cell, so we append to it
    current_cell->next_cell = cell_to_append;
}

void append_row(t_table *table, t_row *row_to_append) {
    if (!table) {
        printf("Table is NULL, can't append row.\n");
        exit(EXIT_FAILURE);
    }
    
    //if there are no rows, append as first one
    if (table->rows == NULL) {
        table->rows = row_to_append;
        return;
    }

    //look for last row of the table
    t_row *current_row = table->rows;
    while (current_row->next_row) {
        current_row = current_row->next_row;
    }
    //here, current_row is the last row, so we append to it
    current_row->next_row = row_to_append;
}

void print_cell(t_cell *cell, int row_index, int cell_index) {
    if (!cell) {
        printf("Cell is NULL, can't print it.\n");
        exit(EXIT_FAILURE);
    }
    if (cell->data->type == CELL_DATA_TYPE_STRING) {
        printf("Cell [%d][%d] { string_data: \"%s\" } ", row_index, cell_index, cell->data->value._string);
    } else if (cell->data->type == CELL_DATA_TYPE_FLOAT) {
        printf("Cell [%d][%d] { float_data: %f } ", row_index, cell_index, cell->data->value._float);
    } else {
        printf("Cell of no type");
    }
}

void print_row(t_row *row, int row_index) {
    printf("Row %d { ", row_index);
    t_cell *cell = row->cells;
    int cell_index = 0;
    while (cell) {
        print_cell(cell, row_index, cell_index);
        cell = cell->next_cell;
        ++cell_index;
    }
    printf("}\n");
}

void print_table(t_table *table) {
    if (!table) {
        printf("Table is NULL, can't print it.\n");
        exit(EXIT_FAILURE);
    }

    t_row *row = table->rows;
    int index = 0;
    while (row) {
        print_row(row, index);
        row = row->next_row;
        ++index;
    }
}

void clear_cell(t_cell* cell) {
    if (cell->data->type == CELL_DATA_TYPE_STRING) free(cell->data->value._string);
    free(cell->data);
}

void clear_row(t_row* row) {
    if (!row) {
        printf("Row is NULL, can't clear it.\n");
        exit(EXIT_FAILURE);
    }

    t_cell* cell = row->cells;
    while (cell) {
        clear_cell(cell);
        t_cell* next_cell = cell->next_cell;
        free(cell);
        cell = next_cell;
    }
}

void clear_table(t_table* table) {
    if (!table) {
        printf("Table is NULL, can't clear it.\n");
        exit(EXIT_FAILURE);
    }

    t_row* row = table->rows;
    while (row) {
        clear_row(row);
        t_row* next_row = row->next_row;
        free(row);
        row = next_row;
    }
}

t_cell* get_cell(t_table* table, int row_index, int cell_index) {
    if (!table) {
        printf("Table is NULL, can't get cell [%d][%d].\n", row_index, cell_index);
        exit(EXIT_FAILURE);
    }

    //find row
    t_row* row = table->rows;
    for (int i = 0; i < row_index; ++i) {
        if (!row) return NULL;
        row = row->next_row;
    }
    if (!row) return NULL;

    //find cell
    t_cell* cell = row->cells;
    for (int j = 0; j < cell_index; ++j) {
        if (!cell) return NULL;
        cell = cell->next_cell;
    }
    if (!cell) return NULL;

    return cell;
}

int table_test() {
    t_table *table = new_table(NULL);
    for (int i = 0; i < 5; ++i) {
        t_row* row = new_row(NULL);
        for (int j = 0; j < 3; ++j) {
            append_cell(row, new_cell(new_cell_data(j)));
        }
        append_row(table, row);
    }

    printf("Testing print_table(table):\n");
    print_table(table);
    printf("\n");

    printf("Testing get_cell(cell, i, j):\n");
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            t_cell* cell = get_cell(table, i, j);
            if (cell) print_cell(cell, i, j);
        }
        printf("\n");
    }

    clear_table(table);
    free(table);

    return 0;
}