#include <cstdio>
#include <iostream>
#include "../include/SpiceInterpreter.h"
#include "../include/SpiceReader.h"
#include "../include/MatrixManager.h"

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
    SpiceInterpreter interpreter = SpiceInterpreter(spiceTable);
    interpreter.interpretSpiceTable();
    bool valid = interpreter.isValidSpiceTable();

    if (valid) {
        print_table(spiceTable); printf("\n\n");
        interpreter.printComponentList();
        interpreter.printNodeMap();
    }

    if (!valid) {
        printf("Found errors in %s, reported in stderr. Correct them and try again.\n", filename);
    }

    auto *manager = new MatrixManager(interpreter.getComponents().size());
    std::cout << *manager;
    manager->requiredPrint();
    delete manager;

    return valid? 0 : 1;
}