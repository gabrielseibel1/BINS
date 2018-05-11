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

class SpiceInterpreter {
private:
    table_t *spiceTable;
    bool validSpiceTable;
    std::vector<component_t> components;
    std::vector<std::string> actions;
    NodeMap nodeMap;

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
    bool isValidComponent(row_t *spice_line, int node_count, int component_type);

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
