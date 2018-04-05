#ifndef PEPPER_INTERPRETER_H
#define PEPPER_INTERPRETER_H

#include <stdbool.h>
#include "table.h"

/**
 * Goes through each row of a t_table and checks if it represents a valid .spc line
 * @param netlist table containing rows that are lines from a .spc file
 */
bool is_valid_spice_table(t_table* netlist);

/**
 * Checks if row of a t_table represents a valid .spc line
 * @param spice_line row that is a line from a .spc file
 */
bool is_valid_spice_row(t_row* spice_line);

/**
 * Checks if a t_row represents a valid command
 */
bool is_valid_command(t_row *command);

/**
 * Checks if a t_row represents a valid command
 */
bool is_valid_component(t_row *component);

/**
 * Prints t_table as lines of a .spc
 * @param netlist table containing rows that are lines from a .spc
 */
void spice_print(t_table* netlist);

/**
 * Tests of the other functions
 */
void interpreter_test();

#endif //PEPPER_INTERPRETER_H
