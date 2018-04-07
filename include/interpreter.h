#ifndef PEPPER_INTERPRETER_H
#define PEPPER_INTERPRETER_H

#include <stdbool.h>
#include "table.h"

/**
 * Goes through each row of a t_table and checks if it represents a valid .spc line
 * @param netlist table containing rows that are lines from a .spc file
 */
bool interpret_spice_table(t_table *netlist);

/**
 * Checks if row of a t_table represents a valid .spc line
 * @param spice_line row that is a line from a .spc file
 */
bool interpret_spice_row(t_row *spice_line);

/**
 * Checks if a t_row represents a valid command
 */
bool is_valid_command(t_row *command);

/**
 * Checks if a t_row represents a valid component
 * @param spice_line row to be analysed
 * @param node_count number of expected "node" cells
 * @param component_type expected type of component (ROW_TYPEs)
 */
bool is_valid_component(t_row *spice_line, int node_count, int component_type);

/**
 * Returns correspondent int node for the given string.
 * If there is no existing matching node, creates a new one (next int)
 */
int get_node_number(u_cell_data_value string);

/**
 * Tests of the other functions
 */
void interpreter_test();

#endif //PEPPER_INTERPRETER_H
