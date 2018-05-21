/**
 * Interprets table_t to parse components, comments and commands
 * Can populate "component" members of row_t
 */

#ifndef BINS_SPICE_INTERPRETER_H
#define BINS_SPICE_INTERPRETER_H

#include <vector>
#include <string>
#include "table.h"
#include "NodeMap.h"
#include "Component.h"

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

class SpiceInterpreter {
private:
    table_t *spiceTable;
    bool validSpiceTable;
    bool simulationRequired;
    std::vector<Component*> components;
    std::vector<std::string> actions;
    NodeMap nodeMap;
    int group1Count;
    int group2Count;

    /**
     * Checks if row of a t_table represents a valid .spc line
     * @param spice_line row that is a line from a .spc file
     */
    bool interpretSpiceRow(row_t *spice_line);

    /**
     * Checks if a t_row represents a valid command
     */
    bool isValidCommand(row_t *row);

    /**
     * Checks if a t_row represents a valid component
     * @param spice_line row to be analysed
     * @param nodeCount number of expected "node" cells
     * @param component_type expected type of component (ROW_TYPEs)
     */
    bool validateAndSaveComponent(row_t *spice_line, int nodeCount, RowType component_type);

    /**
     * Takes a data_t* and checks if it need to be have itś value set to a float multiplied by a unit prefix
     */
    void checkIfDataHasUnitPrefix(data_t *data);

    /**
     * Returns correspondent int node for the given string.
     * If there is no existing matching node, creates a new one (next int)
     */
    int getNodeNumber(data_t *string);

public:
    explicit SpiceInterpreter(table_t *spiceTable);

    virtual ~SpiceInterpreter();

    bool isValidSpiceTable() const;

    bool simulationIsRequired() const;

    const std::vector<Component*> &getComponents() const;

    const std::vector<std::string> &getActions() const;

    /**
     * Goes through each row of a t_table and checks if it represents a valid .spc line
     */
    void interpretSpiceTable();

    void printComponentList();

    void printNodeMap();

    int getGroup1Count() const;

    int getGroup2Count() const;

    const NodeMap &getNodeMap() const;
};

#endif //BINS_SPICE_INTERPRETER_H
