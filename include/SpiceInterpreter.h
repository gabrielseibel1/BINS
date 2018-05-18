/**
 * Interprets table_t to parse components, comments and commands
 * Can populate "component" members of row_t
 */

#ifndef BINS_SPICE_INTERPRETER_H
#define BINS_SPICE_INTERPRETER_H

#include <stdbool.h>
#include <vector>
#include <string>
#include "table.h"
#include "NodeMap.h"

enum UnitPrefix {
    FEMTO = -15,
    PICO = -12,
    NANO = -9,
    MICRO = -6,
    MILLI = -3,
    KILO = 3,
    MEGA = 6,
    GIGA = 9,
    TERA = 12
};

enum Group {
    GROUP1 = 1, GROUP2 = 2
};

typedef struct component {
    RowType type;
    Group group;
    char *label;
    int id;
    int nodes[MAX_NODES];
    data_t *value;
} component_t;

class SpiceInterpreter {
private:
    table_t *spiceTable;
    bool validSpiceTable;
    std::vector<component_t> components;
    std::vector<std::string> actions;
    NodeMap nodeMap;

    /**
     * Constructor for t_component. Builds component structure based on a table row
     * @param spice_line pointer to a t_row containing VALID component information
     */
    component_t *newComponent(RowType type, char *label, const int *nodes, data_t *value);

    /**
     * Checks if row of a t_table represents a valid .spc line
     * @param spice_line row that is a line from a .spc file
     */
    bool interpretSpiceRow(row_t *spice_line);

    /**
     * Checks if a t_row represents a valid command
     */
    bool isValidCommand(row_t *command);

    /**
     * Checks if a t_row represents a valid component
     * @param spice_line row to be analysed
     * @param node_count number of expected "node" cells
     * @param component_type expected type of component (ROW_TYPEs)
     */
    bool isValidComponent(row_t *spice_line, int node_count, RowType component_type);

    /**
     * Takes a data_t* and checks if it need to be have itś value set to a float multiplied by a unit prefix
     */
    void checkIfDataHasUnitPrefix(data_t *data);

    /**
     * Returns correspondent int node for the given string.
     * If there is no existing matching node, creates a new one (next int)
     */
    int getNodeNumber(data_t *string);

    void printComponent(component_t component);

    void setPreliminarGroup(component_t* component);

public:
    explicit SpiceInterpreter(table_t *spiceTable);

    bool isValidSpiceTable() const;

    const std::vector<component_t> &getComponents() const;

    const std::vector<std::string> &getActions() const;

    /**
     * Goes through each row of a t_table and checks if it represents a valid .spc line
     */
    void interpretSpiceTable();

    void printComponentList();

    void printNodeMap();
};

#endif //BINS_SPICE_INTERPRETER_H
