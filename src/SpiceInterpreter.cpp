/**
 * Interprets table_t to parse components, comments and commands
 * Can populate "component" members of row_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "../include/SpiceInterpreter.h"
#include "../include/NodeMap.h"

#define FEMTO -15
#define PICO -12
#define NANO -9
#define MICRO -6
#define MILLI -3
#define KILO 3
#define MEGA 6
#define GIGA 9
#define TERA 12

int componentCount = 0;

SpiceInterpreter::SpiceInterpreter(table_t *spiceTable) : spiceTable(spiceTable) {
    validSpiceTable = false;
}

component_t *newComponent(int type, char *label, const int *nodes, data_t *value) {
    auto *component = (component_t *) malloc(sizeof(component_t));
    component->type = type;
    component->id = componentCount++;
    component->label = label;
    for (int i = 0; i < MAX_NODES; ++i) {
        component->nodes[i] = nodes[i];
    }
    component->value = value;
}

void SpiceInterpreter::interpretSpiceTable() {
    if (!spiceTable) {
        fprintf(stderr, "Table is NULL, can't interpret it.\n");
        exit(EXIT_FAILURE);
    }

    validSpiceTable = true;

    row_t *row = spiceTable->rows;
    while (row) {
        validSpiceTable = interpretSpiceRow(row) && validSpiceTable;
        row = row->next_row;
    }
}

bool SpiceInterpreter::isValidCommand(row_t *command) {
    command->type = TYPE_COMMAND;
    return true;
}

void SpiceInterpreter::checkIfDataHasUnitPrefix(data_t *data) {

    if (data->type == CELL_DATA_TYPE_STRING) {

        char *start_pt = data->value._string;
        char *end_pt;
        double double_data = strtod(data->value._string, &end_pt);

        if (end_pt != NULL && *end_pt != '\0') {// raw_data contained something else after the double
            int exponent = 0;

            if (strcmp(end_pt, "F") == 0) exponent = FEMTO;
            else if (strcmp(end_pt, "P") == 0) exponent = PICO;
            else if (strcmp(end_pt, "N") == 0) exponent = NANO;
            else if (strcmp(end_pt, "U") == 0) exponent = MICRO;
            else if (strcmp(end_pt, "M") == 0) exponent = MILLI;
            else if (strcmp(end_pt, "K") == 0) exponent = KILO;
            else if (strcmp(end_pt, "MEG") == 0) exponent = MEGA;
            else if (strcmp(end_pt, "G") == 0) exponent = GIGA;
            else if (strcmp(end_pt, "T") == 0) exponent = TERA;

            if (exponent != 0) { // found a prefix
                data->type = CELL_DATA_TYPE_DOUBLE;
                data->value._double = double_data * powf(10, exponent);
                free(start_pt);
            }
        }
    }
}

bool SpiceInterpreter::isValidComponent(row_t *spice_line, int node_count, int component_type) {
    cell_t *cell = spice_line->cells;

    //parameters to be parsed so a component_t can be constructed
    char *label = const_cast<char *>("\0");
    int nodes[MAX_NODES] = {UNUSED_NODE, UNUSED_NODE, UNUSED_NODE, UNUSED_NODE};
    data_t *value = NULL;

    for (int i = 0; i < node_count + 2 /*label + nodes + value*/; ++i) {
        if (!cell) {
            fprintf(stderr, "[Line %d] Not enough parameters for component!\n", spice_line->index + 1);
            return false;
        }

        if (i == 0) { //if its first cell, get label (just exclude first char, which represents component type)
            label = strdup(cell->data->value._string + 1);
        } else if (i == node_count + 1) { //last cell contains value
            checkIfDataHasUnitPrefix(cell->data);
            value = cell->data;
        } else { //the current cell is a node
            nodes[i - 1] = getNodeNumber(cell->data);
        }
        cell = cell->next_cell;
    }

    if (cell) {
        fprintf(stderr, "[Line %d] Too many parameters for component!\n", spice_line->index + 1);
        return false;
    }

    component_t *component= newComponent(component_type, label, nodes, value);
    spice_line->type = component_type;
    spice_line->component = component;
    components.insert(components.end(), *component);

    return true;
}

bool SpiceInterpreter::interpretSpiceRow(row_t *spice_line) {
    if (spice_line->index == 0) {
        spice_line->type = TYPE_COMMENT;
        return true;
    }

    cell_t *first_cell = spice_line->cells;
    if (first_cell->data->type == CELL_DATA_TYPE_STRING) {

        //analyse first character of the line
        char first_char = first_cell->data->value._string[0];

        switch (first_char) {
            case '*': //comment
                //printf("Comment\n");
                spice_line->type = TYPE_COMMENT;
                return true; //any comment is valid

            case '.': //command
                //printf("Command\n");
                //TODO validate inputs like .5, so that they don't get confused with 0.5
                return isValidCommand(spice_line);

            case 'C': //Capacitor
                //printf("Capacitor\n");
                return isValidComponent(spice_line, 2, TYPE_C);
            case 'D': //Diode
                //printf("Diode\n");
                return isValidComponent(spice_line, 2, TYPE_D);
            case 'E': //VCVS - voltage controlled voltage source
                //printf("VCVS\n");
                return isValidComponent(spice_line, 4, TYPE_E);
            case 'F': //CCCS - current controlled current source
                //printf("CCCS\n");
                return isValidComponent(spice_line, 4, TYPE_F);
            case 'G': //VCCS - voltage controlled current source
                //printf("VCCS\n");
                return isValidComponent(spice_line, 4, TYPE_G);
            case 'H': //CCVS - current controlled voltage source
                //printf("CCVS\n");
                return isValidComponent(spice_line, 4, TYPE_H);
            case 'I': //Current source
                //printf("Current source"\n);
                return isValidComponent(spice_line, 2, TYPE_I);
            case 'L': //Inductor
                //printf("Inductor\n");
                return isValidComponent(spice_line, 2, TYPE_L);
            case 'M': //MOSFET
                //printf("MOSFET\n");
                return isValidComponent(spice_line, 3, TYPE_M);
            case 'Q': //BJT - binary junction transistor
                //printf("BJT\n");
                return isValidComponent(spice_line, 3, TYPE_Q);
            case 'R': //Resistor
                //printf("Resistor\n");
                return isValidComponent(spice_line, 2, TYPE_R);
            case 'V': //V - Voltage source
                //printf("Voltage source\n");
                return isValidComponent(spice_line, 2, TYPE_V);

            default:
                fprintf(stderr, "[Line %d] Unexpected '%c' as first character of line!\n", spice_line->index + 1,
                        first_char);
                return false;
        }
    } else {
        fprintf(stderr, "[Line %d] Unexpected '%lf' starting the line!\n", spice_line->index + 1,
                first_cell->data->value._double);
        return false;
    }
}


void SpiceInterpreter::printComponent(component_t component) {
    printf("TYPE: %s\t\t", row_type_to_string(component.type));
    printf("ID: %d\t\t", component.id);
    printf("LABEL: %s\t\t", component.label);
    for (int i = 0; i < MAX_NODES; ++i) {
        if (component.nodes[i] != UNUSED_NODE) printf("NODE%d:\t%d\t", i, component.nodes[i]);
    }
    printf("VALUE: ");
    print_cell_data(component.value);
    printf("\n");
}

void SpiceInterpreter::printComponentList() {
    printf("\nCOMPONENTS:\n");

    for (auto component : components) {
        printComponent(component);
    }
}

int SpiceInterpreter::getNodeNumber(data_t *string) {
    return nodeMap.nodeToInt(string);
}

bool SpiceInterpreter::isValidSpiceTable() const {
    return validSpiceTable;
}

const std::vector<component_t> &SpiceInterpreter::getComponents() const {
    return components;
}

const std::vector<std::string> &SpiceInterpreter::getActions() const {
    return actions;
}

void SpiceInterpreter::printNodeMap() {
    std::cout << nodeMap;
}


