#include <cstdio>
#include <iostream>
#include "../include/SpiceInterpreter.h"
#include "../include/SpiceReader.h"
#include "TranSolver.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Too %s arguments for BINS. Expected 2 (input, output), got %d.", (argc < 3) ? "few" : "many", argc - 1);
        return -1;
    }

    char *inputFilename = argv[1];
    char *outputFilename = argv[2];

    //read table
    SpiceReader reader = SpiceReader(inputFilename);
    reader.read();
    table_t *spiceTable = reader.getSpiceTable();

    //validate table and extract info from it
    auto *interpreter = new SpiceInterpreter(spiceTable);
    interpreter->interpretSpiceTable();
    bool valid = interpreter->isValidSpiceTable();

    if (valid) {
        print_table(spiceTable);
        printf("\n\n");
        interpreter->printComponentList();
        interpreter->printNodeMap();
    }

    if (!valid) {
        printf("Found errors in %s, reported in stderr. Correct them and try again.\n", inputFilename);
        exit(EXIT_FAILURE);
    }

    if (interpreter->OPIsRequired()) {
        NodeMap *nodeMap = interpreter->getNodeMap();
        OPSolver solver = OPSolver(
                nodeMap->getSize() - 1 /*don't count GND*/ + static_cast<size_t>(interpreter->getGroup2Count()),
                nodeMap->getSize() - 1 /*don't count GND*/);

        solver.accumulativeStamp(interpreter->getComponents());
        solver.solveOP();
        solver.interpretedPrint(interpreter);


    } else if (interpreter->tranIsRequired()) {
        TranSolver tranSolver = TranSolver(interpreter);
        std::cout << "\n\nTransient analysis ...\n";
        tranSolver.solveTransient();
        tranSolver.printSolutions(outputFilename);
        std::cout << "Results written to " << outputFilename << "\n";
    } else {
        std::cout
                << "\n\nTo simulate the circuit, insert the command .OP or .TRAN <step> <max time> in your spice file.\n";
    }

    delete interpreter;
    return valid ? 0 : 1;
}