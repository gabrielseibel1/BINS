#include <cstdio>
#include <iostream>
#include "../include/SpiceInterpreter.h"
#include "../include/SpiceReader.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Too %s arguments for BINS. Expected 1 (filename), got %d.", (argc < 2) ? "few" : "many", argc - 1);
        return -1;
    }

    char* filename = argv[1];

    //read table
    SpiceReader reader = SpiceReader(filename);
    reader.read();
    table_t* spiceTable = reader.getSpiceTable();

    //validate table and extract info from it
    auto *interpreter = new SpiceInterpreter(spiceTable);
    interpreter->interpretSpiceTable();
    bool valid = interpreter->isValidSpiceTable();

    if (valid) {
        print_table(spiceTable); printf("\n\n");
        interpreter->printComponentList();
        interpreter->printNodeMap();
    }

    if (!valid) {
        printf("Found errors in %s, reported in stderr. Correct them and try again.\n", filename);
    }

    if (interpreter->simulationIsRequired()) {
        NodeMap nodeMap = interpreter->getNodeMap();
        Solver solver = Solver(nodeMap.getSize() - 1 /*don't count GND*/ + static_cast<size_t>(interpreter->getGroup2Count()),
                               nodeMap.getSize() - 1 /*don't count GND*/);

        solver.stamp(interpreter->getComponents());
        solver.solveOP();
        solver.interpretedPrint(interpreter);


    } else {
        std::cout << "\n\nTo simulate the circuit, insert the command \".OP\" in your spice file.\n";
    }

    delete interpreter;
    return valid? 0 : 1;
}