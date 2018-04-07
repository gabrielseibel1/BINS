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
#define TYPE_COMMENT 0
#define TYPE_COMMAND 1
#define TYPE_C 2
#define TYPE_D 3
#define TYPE_E 4
#define TYPE_F 5
#define TYPE_G 6
#define TYPE_H 7
#define TYPE_L 8
#define TYPE_M 9
#define TYPE_Q 10
#define TYPE_R 11
#define TYPE_V 12

#define UNUSED_NODE (-1)
#define MAX_NODES 4

typedef union cell_data_value {
    char* _string;
    float _float;
} cell_data_value_u;

/**
 * Types od data that a cell can store
 */
typedef struct cell_data {
    int type;
    cell_data_value_u value;
} data_t;

/**
 * Represents a table cell, containing data and a pointer to the next cell of the row
 */
typedef struct cell {
    data_t* data;
    struct cell *next_cell;
} cell_t;

typedef struct component {
    int type;
    char* label;
    int nodes[MAX_NODES];
    data_t* value;
} component_t;

/**
 * Represents a row with a list of cells and a pointer to the next row of the table
 */
typedef struct row {
    int type;
    int index;
    cell_t *cells;
    component_t* component;
    struct row *next_row;
} row_t;

/**
 * Represents a table with a list of rows
 */
typedef struct table {
    row_t *rows;
} table_t;

/**
 * Constructor for u_cell_data.
 * @param data The data, of any (valid) type, to be saved in the u_cell_data
 * @param type The type to cast the data to
 */
data_t* new_cell_data(char* raw_data);

/**
 * Constructor for t_cell.
 */
cell_t *new_cell(data_t* data);

/**
 * Constructor for t_component. Builds component structure based on a table row
 * @param spice_line pointer to a t_row containing VALID component information
 */
component_t* new_component(int type, char* label, const int nodes[MAX_NODES], data_t* value);

/**
 * Constructor for t_row. Accepts a NULL first_cell, if user wants to make an empty row.
 */
row_t *new_row(cell_t *first_cell);

/**
 * Constructor for t_table. Accepts a NULL first_row, if user wants to make an empty table.
 */
table_t *new_table(row_t *first_row);

/**
 * Appends a cell to the end of a row, making it the last cell.
 */
void append_cell(row_t *row, cell_t *cell_to_append);

/**
 * Appends a row to the end of a table, making it the last row.
 */
void append_row(table_t *table, row_t *row_to_append);

/**
 * Gets string for type of row (component(s), command and comment)
 */
char* row_type_to_string(int type);

/**
 * Prints t_cell_data as string or float depending on type
 * @param cell
 */
void print_cell_data(const data_t *cell);

/**
 * Prints a t_cell.
 * @param cell_index is the index of the cell in it's row
 */
void print_cell(cell_t *cell);

/**
 * Prints a component
 */
void print_component(component_t* component);

/**
 * Prints all the cells of the row, evoking print_cell(cell, cell_number) for each.
 * @param row_index is the index of the row in it's table
 */
void print_row(row_t *row);

/**
 * Prints all of the rows of the table, evoking print_row(row, row_index) for each.
 */
void print_table(table_t *table);

/**
 * Prints a list of components, represented by a t_table*
 */
void print_table_as_component_list(table_t* list_of_components);

/**
 * Clears allocated data from a cell
 */
void clear_cell(cell_t* cell);

/**
 * Clears (evokes clear_cell(cell)) and frees every cell of the row
 */
void clear_row(row_t* row);

/**
 * Clears (evokes clear_row(row)) and frees every row of the table
 */
void clear_table(table_t* table);

/**
 * Returns the table cell specified by the row and cell indexes. If it cannot find the cell, returns NULL.
 * @param row_index the number of the line containing the cell (starts from 0)
 * @param cell_index the number of the column containing the cell (starts from 0)
 * @return returns pointer to cell, if it can be found, or else returns NULL
 */
cell_t* get_cell(table_t* table, int row_index, int cell_index);

#endif //PEPPER_TABLE_H
