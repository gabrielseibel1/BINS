//
// Created by gabriel on 4/5/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/interpreter.h"
#include "../include/lexer.h"

t_component* new_component(int type, char* label, const int nodes[MAX_NODES], t_cell_data* value) {
    t_component* component = (t_component*) malloc(sizeof(t_component));
    component->type = type;
    component->label = label;
    for (int i = 0; i < MAX_NODES; ++i) {
        component->nodes[i] = nodes[i];
    }
    component->value = value;
}

bool interpret_spice_table(t_table *spice_table) {
    if (!spice_table) {
        printf("Table is NULL, can't interpret it.\n");
        exit(EXIT_FAILURE);
    }

    bool table_is_valid = true;

    t_row *row = spice_table->rows;
    while (row) {
        table_is_valid = interpret_spice_row(row) && table_is_valid;
        row = row->next_row;
    }

    return table_is_valid;
}

bool is_valid_command(t_row *command) {
    command->type = TYPE_COMMAND;
    return true;
}

bool is_valid_component(t_row *spice_line, int node_count, int component_type) {
    t_cell* cell = spice_line->cells;

    //parameters to be parsed so a t_component can be constructed
    char* label = "\0";
    int nodes[MAX_NODES] = {UNUSED_NODE, UNUSED_NODE, UNUSED_NODE, UNUSED_NODE};
    t_cell_data* value = NULL;

    for (int i = 0; i < node_count + 2 /*label + nodes + value*/; ++i) {
        if (!cell) {
            printf("[Line %d] Not enough parameters for component!\n", spice_line->index + 1);
            return false;
        }

        if (i == 0) { //if its first cell, get label (just exclude first char, which represents component type)
            label = strdup(cell->data->value._string + 1);
        } else if (i == node_count + 1) { //last cell contains value
            value = cell->data;
        } else { //the current cell is a node
            nodes[i - 1] = get_node_number(cell->data->value);
        }
        cell = cell->next_cell;
    }

    if (cell) {
        printf("[Line %d] Too many parameters for component!\n", spice_line->index + 1);
        return false;
    }

    spice_line->type = component_type;
    spice_line->component = new_component(
            component_type,
            label,
            nodes,
            value
    );
    return true;
}

bool interpret_spice_row(t_row *spice_line) {
    t_cell* first_cell = spice_line->cells;
    if (first_cell->data->type == CELL_DATA_TYPE_STRING) {

        //analyse first character of the line
        char first_char = first_cell->data->value._string[0];

        switch (first_char) {
            case '*': //comment
                printf("Comment\n");
                spice_line->type = TYPE_COMMENT;
                return true; //any comment is valid

            case '.': //command
                printf("Command\n");
                //TODO validate inputs like .5, so that they don't get confused with 0.5
                return is_valid_command(spice_line);

            case 'C': //Capacitor
                printf("Capacitor\n");
                return is_valid_component(spice_line, 2, TYPE_C);
            case 'D': //Diode
                printf("Diode\n");
                return is_valid_component(spice_line, 2, TYPE_D);
            case 'E': //VCVS - voltage controlled voltage source
                printf("VCVS\n");
                return is_valid_component(spice_line, 4, TYPE_E);
            case 'F': //CCCS - current controlled current source
                printf("CCCS\n");
                return is_valid_component(spice_line, 4, TYPE_F);
            case 'G': //VCCS - voltage controlled current source
                printf("VCCS\n");
                return is_valid_component(spice_line, 4, TYPE_G);
            case 'H': //CCVS - current controlled voltage source
                printf("CCVS\n");
                return is_valid_component(spice_line, 4, TYPE_H);
            case 'L': //Inductor
                printf("Inductor\n");
                return is_valid_component(spice_line, 2, TYPE_L);
            case 'M': //MOSFET -
                printf("MOSFET\n");
                return is_valid_component(spice_line, 3, TYPE_M);
            case 'Q': //BJT - binary junction transistor
                printf("BJT\n");
                return is_valid_component(spice_line, 3, TYPE_Q);
            case 'R': //Resistor
                printf("Resistor\n");
                return is_valid_component(spice_line, 2, TYPE_R);
            case 'V': //Source (voltage or current)
                printf("Voltage or current source\n");
                return is_valid_component(spice_line, 2, TYPE_V);

            default:
                printf("[Line %d] Unexpected '%c' as first character of line!\n", spice_line->index + 1, first_char);
                return false;
        }
    } else {
        printf("[Line %d] Unexpected '%f' starting the line!\n", spice_line->index + 1, first_cell->data->value._float);
        return false;
    }
}

void print_table_as_component_list(t_table* list_of_components) {
    if (!list_of_components) {
        printf("Table is NULL, can't print it.\n");
        exit(EXIT_FAILURE);
    }

    t_row *row = list_of_components->rows;
    while (row) {
        if (row->component) print_component(row->component);
        row = row->next_row;
    }

}
void print_component(t_component* component) {
    printf("TYPE: %s\t\t", row_type_to_string(component->type));
    printf("LABEL: %s\t\t", component->label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (component->nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, component->nodes[i]);
    }
    printf("VALUE: ");
    print_cell_data(component->value);
    printf("\n");
}

int get_node_number(u_cell_data_value string) {
    return 0; //TODO change
}

void interpreter_test() {
    //read table
    printf("netlist.spc:\n");
    t_table* table_spc = lex("../data/netlist.spc");

    //validate table and pre-format it
    bool valid = interpret_spice_table(table_spc);
    printf("\n\nValid netlist: "); printf(valid ? "true\n" : "false\n");

    if (valid) {
        print_table(table_spc); printf("\n\n");
        print_table_as_component_list(table_spc);
    }
    clear_table(table_spc);
    free(table_spc);
}
