#include <stdio.h>
#include <stdlib.h>
#include "../include/interpreter.h"
#include "../include/lexer.h"
#include "../include/node_map.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Too %s arguments for BINS. Expected 1 (filename), got %d.", (argc < 2) ? "few" : "many", argc - 1);
        return -1;
    }

    char* filename = argv[1];

    //read table
    table_t* table_spc = lex(filename);

    //validate table and pre-format it
    bool valid = interpret_spice_table(table_spc);

    if (valid) {
        print_table(table_spc); printf("\n\n");
        print_table_as_component_list(table_spc);
        print_node_map();
    }

    clear_table(table_spc);
    free(table_spc);
    clear_node_map();

    if (!valid) {
        printf("Found errors in %s, reported in stderr. Correct them and try again.\n", filename);
    }

    return valid? 0 : 1;
}