//
// Created by gabriel on 4/5/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/interpreter.h"
#include "../include/lexer.h"

bool is_valid_component(t_row *component);

bool is_valid_spice_table(t_table* netlist) {
    if (!netlist) {
        printf("Table is NULL, can't interpret it.\n");
        exit(EXIT_FAILURE);
    }

    bool is_valid = true;

    t_row *row = netlist->rows;
    int index = 0;
    while (row) {
        is_valid = is_valid_spice_row(row) && is_valid;
        row = row->next_row;
        ++index;
    }
}

bool is_valid_command(t_row *command) {
    return 0;
}

bool is_valid_component(t_row *component) {
    return 0;
}

bool is_valid_spice_row(t_row* spice_line) {
    t_cell first_cell = spice_line->cells[0];
    if (first_cell.data->type == CELL_DATA_TYPE_STRING) {

        //analyse first character of the line
        char first_char = first_cell.data->value._string[0];

        switch (first_char) {
            default:
            //found comment
            case '*':
                printf("Comment\n");
                //any comment is valid
                return true;

            //found command
            case '.':
                printf("Command\n");
                return is_valid_command(spice_line);

            //TODO add other component letters
            case 'C':
            case 'R':
                printf("Component\n");
                return is_valid_component(spice_line);

        }
    }
}

void interpreter_test() {
    printf("netlist.spc:\n");
    t_table* table_spc = lex("../data/netlist.spc");
    is_valid_spice_table(table_spc);
    print_table(table_spc);
    clear_table(table_spc);
    free(table_spc);
}

