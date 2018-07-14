/**
 * Interprets table_t to parse components, comments and commands
 */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "../include/SpiceInterpreter.h"

SpiceInterpreter::SpiceInterpreter(table_t *spiceTable) : spiceTable(spiceTable) {
    validSpiceTable = false;
    opRequired = false;
    tranRequired = false;
    tranStep = 0;
    tranMaxTime = 0;
    group1Count = 0;
    group2Count = 0;
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
    for (auto *component : components) {
        Component *controller = component->getControllerIfExists(components);
        if (controller) {
            controller->group = GROUP2;
        }
    }

    for (auto *component : components) {
        if (component->group == GROUP1)
            component->indexInGroup = group1Count++;
        else
            component->indexInGroup = group2Count++;
    }
}

bool SpiceInterpreter::isValidCommand(row_t *row) {
    row->type = CMD;
    if (strcmp(row->cells->data->value._string, ".OP") == 0) {
        opRequired = true;
    }
    if (strcmp(row->cells->data->value._string, ".TRAN") == 0) {

        //assert time step specified
        auto stepCell = row->cells->next_cell;
        if (stepCell) {
            checkIfDataHasUnitPrefix(stepCell->data, 0);
            tranStep = stepCell->data->value._double;

            //assert maxTime specified
            auto maxTimeCell = stepCell->next_cell;
            if (maxTimeCell) {
                checkIfDataHasUnitPrefix(maxTimeCell->data, 0);
                tranMaxTime = maxTimeCell->data->value._double;

                //can do .TRAN
                tranRequired = true;
            }
        }
    }
    return true;
}

/**
 * Search for "SIN(" or "PWL(" and set sineParams or PWLParams if needed
 */
RowType SpiceInterpreter::checkIfCellHasSpecialVSource(cell_t *cell, SineParams **sineParams, PWLParams **pwlParams) {
    RowType type = V;
    data_t *data = cell->data;

    if (data->type == CELL_DATA_TYPE_STRING) {

        //get first three chars
        char prefix[5] = { 'x', 'x', 'x', 'x', '\0'};
        strncpy(prefix, data->value._string, 4);

        //compare with "SIN("
        if (strcmp(prefix, "SIN(") == 0) {
            type = S;

            //fill sine parameters
            *sineParams = static_cast<SineParams *>(malloc(sizeof(SineParams)));
            for (int i = 0; i < 4; ++i) { //get for SIN parameters

                //get double value
                checkIfDataHasUnitPrefix(cell->data, /*offset for "SIN("*/(i == 0) ? 4 : 0);
                double value = cell->data->value._double;

                switch (i) {
                    case 0: (*sineParams)->dcOffset = value; break;
                    case 1: (*sineParams)->amplitude = value; break;
                    case 2: (*sineParams)->frequency = value; break;
                    case 3: (*sineParams)->phase = value; break;
                    default: break;
                }

                cell = cell->next_cell;
            }

        //compare with "PWL("
        } else if (strcmp(prefix, "PWL(") == 0) {
            type = P;

            //fill pwl parameters
            *pwlParams = static_cast<PWLParams *>(malloc(sizeof(PWLParams)));
            (*pwlParams)->timeVoltagePairs = new std::vector<std::pair<double, double>>;

            int pointCount = 0;
            while (cell) {
                //get double value for time
                checkIfDataHasUnitPrefix(cell->data, /*offset for "PWL("*/(pointCount == 0) ? 4 : 0);
                double time = cell->data->value._double;
                cell = cell->next_cell;

                //get double value for voltage
                checkIfDataHasUnitPrefix(cell->data, 0);
                double voltage = cell->data->value._double;
                cell = cell->next_cell;

                //add pair to vector
                (*pwlParams)->timeVoltagePairs->emplace_back(std::make_pair(time, voltage));

                pointCount++;
            }
        }
    }

    return type;
}

void SpiceInterpreter::checkIfDataHasInitialCondition(data_t *data, double *pInitialCondition) {
    if (data->type == CELL_DATA_TYPE_STRING) {

        //get first three chars
        char initial_condition_prefix[4] = { 'x', 'x' , 'x', '\0'};
        strncpy(initial_condition_prefix, data->value._string, 3);

        //compare with "IC="
        if (strcmp(initial_condition_prefix, "IC=") != 0) {
            data->value._double = 0;
            return;
        }

        char *start_pt = data->value._string;
        char *end_pt;
        double double_data = strtod(data->value._string + 3, &end_pt);

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

        *pInitialCondition = data->value._double;
    }
}

void SpiceInterpreter::checkIfDataHasUnitPrefix(data_t *data, int offset) {

    if (data->type == CELL_DATA_TYPE_STRING) {
        char *start_pt = data->value._string;
        char *end_pt;
        double double_data = strtod(data->value._string + offset, &end_pt);

        int exponent = 0;
        if (end_pt != nullptr && *end_pt != '\0') {// raw_data contained something else after the double

            if (strcmp(end_pt, "F") == 0)           exponent = FEMTO;
            else if (strcmp(end_pt, "P") == 0)      exponent = PICO;
            else if (strcmp(end_pt, "N") == 0)      exponent = NANO;
            else if (strcmp(end_pt, "U") == 0)      exponent = MICRO;
            else if (strcmp(end_pt, "M") == 0)      exponent = MILLI;
            else if (strcmp(end_pt, "K") == 0)      exponent = KILO;
            else if (strcmp(end_pt, "MEG") == 0)    exponent = MEGA;
            else if (strcmp(end_pt, "G") == 0)      exponent = GIGA;
            else if (strcmp(end_pt, "T") == 0)      exponent = TERA;
            else if (strcmp(end_pt, ")") == 0)      exponent = 0;
            else {
                fprintf(stderr, "Unexpected %s!\n", end_pt);
                exit(EXIT_FAILURE);
            }
        }

        data->type = CELL_DATA_TYPE_DOUBLE;
        data->value._double = double_data * powf(10, exponent);
        free(start_pt);
    }
}

bool SpiceInterpreter::validateAndSaveComponent(row_t *spice_line, int nodeCount, RowType component_type) {
    cell_t *cell = spice_line->cells;

    //parameters to be parsed so a component_t can be constructed
    char *label = nullptr;
    int nodes[MAX_NODES] = {UNUSED_NODE, UNUSED_NODE, UNUSED_NODE, UNUSED_NODE};
    data_t *value = nullptr;
    char* controllerCurrent = nullptr;
    double initialCondition = 0;
    SineParams *sineParams = nullptr;
    PWLParams *pwlParams = nullptr;
    RowType specialVSourceType = V;

    for (int i = 0; i < nodeCount + 2 /*label + nodes + value*/; ++i) {
        if (!cell) {
            fprintf(stderr, "[Line %d] Not enough parameters for component!\n", spice_line->index + 1);
            return false;
        }

        if (i == 0) { //if its first cell, get label
            label = strdup(cell->data->value._string);
        } else if (i == nodeCount + 1) { //last cell contains value

            if (component_type == V) { //search for SIN or PWL
                specialVSourceType = checkIfCellHasSpecialVSource(cell, &sineParams, &pwlParams);
            }

            //get double value
            checkIfDataHasUnitPrefix(cell->data, 0);
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

    if (cell && specialVSourceType == V) { //too many cells?
        if (component_type == C || component_type == L) { //check for initial conditions in extra cell
            checkIfDataHasInitialCondition(cell->data, &initialCondition);

            if (cell->next_cell) {
                fprintf(stderr, "[Line %d] Too many parameters for component!\n", spice_line->index + 1);
                return false;
            }
        } else {
            fprintf(stderr, "[Line %d] Too many parameters for component!\n", spice_line->index + 1);
            return false;
        }
    }

    ComponentFactory factory = ComponentFactory();
    Component *component = factory.createComponent((specialVSourceType != V) ? specialVSourceType: component_type,
            label, nodes, value, controllerCurrent, initialCondition, sineParams, pwlParams);
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

    for (Component *component : components) {
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

int SpiceInterpreter::getGroup1Count() const {
    return group1Count;
}

int SpiceInterpreter::getGroup2Count() const {
    return group2Count;
}

NodeMap *SpiceInterpreter::getNodeMap() {
    return &nodeMap;
}

bool SpiceInterpreter::OPIsRequired() const {
    return opRequired;
}

bool SpiceInterpreter::tranIsRequired() const {
    return tranRequired;
}
