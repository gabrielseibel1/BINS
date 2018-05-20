/**
 * Interprets table_t to parse components, comments and commands
 */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "../include/SpiceInterpreter.h"

int componentCount = 0;

SpiceInterpreter::SpiceInterpreter(table_t *spiceTable) : spiceTable(spiceTable) {
    validSpiceTable = false;
}

void SpiceInterpreter::interpretSpiceTable() {
    if (!spiceTable) {
        fprintf(stderr, "Table is nullptr, can't interpret it.\n");
        exit(EXIT_FAILURE);
    }

    validSpiceTable = true;

    //interpret each row
    row_t *row = spiceTable->rows;
    while (row) {
        validSpiceTable = interpretSpiceRow(row) && validSpiceTable;
        row = row->next_row;
    }


    //check for group2 elements
    std::vector<std::string> group2Members = std::vector<std::string>();
    for (auto *component : components) {
        component->setControllerToGroup2IfControlled(components);
    }
}

bool SpiceInterpreter::isValidCommand(row_t *command) {
    command->type = CMD;
    return true;
}

void SpiceInterpreter::checkIfDataHasUnitPrefix(data_t *data) {

    if (data->type == CELL_DATA_TYPE_STRING) {

        char *start_pt = data->value._string;
        char *end_pt;
        double double_data = strtod(data->value._string, &end_pt);

        if (end_pt != nullptr && *end_pt != '\0') {// raw_data contained something else after the double
            int exponent = 0;

            if (strcmp(end_pt, "F") == 0)           exponent = FEMTO;
            else if (strcmp(end_pt, "P") == 0)      exponent = PICO;
            else if (strcmp(end_pt, "N") == 0)      exponent = NANO;
            else if (strcmp(end_pt, "U") == 0)      exponent = MICRO;
            else if (strcmp(end_pt, "M") == 0)      exponent = MILLI;
            else if (strcmp(end_pt, "K") == 0)      exponent = KILO;
            else if (strcmp(end_pt, "MEG") == 0)    exponent = MEGA;
            else if (strcmp(end_pt, "G") == 0)      exponent = GIGA;
            else if (strcmp(end_pt, "T") == 0)      exponent = TERA;

            if (exponent != 0) { // found a prefix
                data->type = CELL_DATA_TYPE_DOUBLE;
                data->value._double = double_data * powf(10, exponent);
                free(start_pt);
            }
        }
    }
}

bool SpiceInterpreter::validateAndSaveComponent(row_t *spice_line, int nodeCount, RowType component_type) {
    cell_t *cell = spice_line->cells;

    //parameters to be parsed so a component_t can be constructed
    char *label = nullptr;
    int nodes[MAX_NODES] = {UNUSED_NODE, UNUSED_NODE, UNUSED_NODE, UNUSED_NODE};
    data_t *value = nullptr;
    char* controllerCurrent = nullptr;

    for (int i = 0; i < nodeCount + 2 /*label + nodes + value*/; ++i) {
        if (!cell) {
            fprintf(stderr, "[Line %d] Not enough parameters for component!\n", spice_line->index + 1);
            return false;
        }

        if (i == 0) { //if its first cell, get label
            label = strdup(cell->data->value._string);
        } else if (i == nodeCount + 1) { //last cell contains value
            checkIfDataHasUnitPrefix(cell->data);
            value = cell->data;
        } else { //the current cell is a node
            //current-controlled sources have third "node" as label for controller-current element
            if ((component_type == F || component_type == H) && i == nodeCount) {
                controllerCurrent = strdup(cell->data->value._string);
            } else {
                nodes[i - 1] = getNodeNumber(cell->data);
            }
        }
        cell = cell->next_cell;
    }

    if (cell) {
        fprintf(stderr, "[Line %d] Too many parameters for component!\n", spice_line->index + 1);
        return false;
    }

    ComponentFactory factory = ComponentFactory();
    Component *component = factory.createComponent(component_type, label, componentCount++, nodes, value, controllerCurrent);
    spice_line->type = component_type;
    components.insert(components.end(), component);

    return true;
}

bool SpiceInterpreter::interpretSpiceRow(row_t *spice_line) {
    if (spice_line->index == 0) {
        spice_line->type = CMT;
        return true;
    }

    cell_t *first_cell = spice_line->cells;
    if (first_cell->data->type == CELL_DATA_TYPE_STRING) {

        //analyse first character of the line
        char first_char = first_cell->data->value._string[0];

        switch (first_char) {
            case '*': //comment
                spice_line->type = CMT;
                return true; //any comment is valid

            case '.': //command
                //TODO validate inputs like .5, so that they don't get confused with 0.5
                return isValidCommand(spice_line);

            case 'C': //Capacitor
                return validateAndSaveComponent(spice_line, 2, C);
            case 'D': //Diode
                return validateAndSaveComponent(spice_line, 2, D);
            case 'E': //VCVS - voltage controlled voltage source
                return validateAndSaveComponent(spice_line, 4, E);
            case 'F': //CCCS - current controlled current source
                return validateAndSaveComponent(spice_line, 3, F);
            case 'G': //VCCS - voltage controlled current source
                return validateAndSaveComponent(spice_line, 4, G);
            case 'H': //CCVS - current controlled voltage source
                return validateAndSaveComponent(spice_line, 3, H);
            case 'I': //Current source
                return validateAndSaveComponent(spice_line, 2, I);
            case 'L': //Inductor
                return validateAndSaveComponent(spice_line, 2, L);
            case 'M': //MOSFET
                return validateAndSaveComponent(spice_line, 3, M);
            case 'Q': //BJT - binary junction transistor
                return validateAndSaveComponent(spice_line, 3, Q);
            case 'R': //Resistor
                return validateAndSaveComponent(spice_line, 2, R);
            case 'V': //V - Voltage source
                return validateAndSaveComponent(spice_line, 2, V);

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

void SpiceInterpreter::printComponentList() {
    printf("\nCOMPONENTS:\n");

    for (auto &component : components) {
        component->print();
    }
}

int SpiceInterpreter::getNodeNumber(data_t *string) {
    return nodeMap.nodeToInt(string);
}

bool SpiceInterpreter::isValidSpiceTable() const {
    return validSpiceTable;
}

const std::vector<Component*> &SpiceInterpreter::getComponents() const {
    return components;
}

const std::vector<std::string> &SpiceInterpreter::getActions() const {
    return actions;
}

void SpiceInterpreter::printNodeMap() {
    std::cout << nodeMap;
}

SpiceInterpreter::~SpiceInterpreter() {
    for (auto *component : components) {
        delete component;
    }
}
