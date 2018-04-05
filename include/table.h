/**
 * Structs and headers for table.c
 * Defines a table, made of rows (which are made of cells). Each cell of the table contains data.
 * Defines structs, constructors, insertions, accesses (getter), printing and clearing for the data structures.
 */

#ifndef PEPPER_TABLE_H
#define PEPPER_TABLE_H

#include <string.h>

#define CELL_DATA_TYPE_STRING 0
#define CELL_DATA_TYPE_FLOAT 1

typedef union cell_data_value {
    char* _string;
    float _float;
} u_cell_data_value;

/**
 * Types od data that a cell can store
 */
typedef struct cell_data {
    int type;
    u_cell_data_value value;
} t_cell_data;

/**
 * Represents a table cell, containing data and a pointer to the next cell of the row
 */
typedef struct cell {
    t_cell_data* data;
    struct cell *next_cell;
} t_cell;

/**
 * Represents a row with a list of cells and a pointer to the next row of the table
 */
typedef struct row {
    t_cell *cells;
    struct row *next_row;
} t_row;

/**
 * Represents a table with a list of rows
 */
typedef struct table {
    t_row *rows;
} t_table;

/**
 * Constructor for u_cell_data.
 * @param data The data, of any (valid) type, to be saved in the u_cell_data
 * @param type The type to cast the data to
 */
t_cell_data* new_cell_data(char* raw_data);

/**
 * Constructor for t_cell.
 */
t_cell *new_cell(t_cell_data* data);

/**
 * Constructor for t_row. Accepts a NULL first_cell, if user wants to make an empty row.
 */
t_row *new_row(t_cell *first_cell);

/**
 * Constructor for t_table. Accepts a NULL first_row, if user wants to make an empty table.
 */
t_table *new_table(t_row *first_row);

/**
 * Appends a cell to the end of a row, making it the last cell.
 */
void append_cell(t_row *row, t_cell *cell_to_append);

/**
 * Appends a row to the end of a table, making it the last row.
 */
void append_row(t_table *table, t_row *row_to_append);

/**
 * Prints a t_cell.
 * @param cell_index is the index of the cell in it's row
 */
void print_cell(t_cell *cell, int row_index, int cell_index);

/**
 * Prints all the cells of the row, evoking print_cell(cell, cell_number) for each.
 * @param row_index is the index of the row in it's table
 */
void print_row(t_row *row, int row_index);

/**
 * Prints all of the rows of the table, evoking print_row(row, row_index) for each.
 */
void print_table(t_table *table);

/**
 * Clears allocated data from a cell
 */
void clear_cell(t_cell* cell);

/**
 * Clears (evokes clear_cell(cell)) and frees every cell of the row
 */
void clear_row(t_row* row);

/**
 * Clears (evokes clear_row(row)) and frees every row of the table
 */
void clear_table(t_table* table);

/**
 * Returns the table cell specified by the row and cell indexes. If it cannot find the cell, returns NULL.
 * @param row_index the number of the line containing the cell (starts from 0)
 * @param cell_index the number of the column containing the cell (starts from 0)
 * @return returns pointer to cell, if it can be found, or else returns NULL
 */
t_cell* get_cell(t_table* table, int row_index, int cell_index);

#endif //PEPPER_TABLE_H
