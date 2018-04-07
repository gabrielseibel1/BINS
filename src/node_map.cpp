/**
 * Helper file to map nodes to ints
 */

#include "../include/node_map.h"
#include <cmath>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>

std::map<std::string, int> node_map;
int last_node_int;

int next_node_int() {
    return ++last_node_int;
}

int node_to_int(data_t* node) {

    std::string node_as_string;

    if (node->type == CELL_DATA_TYPE_STRING) {
        node_as_string.assign(node->value._string);
    } else if (node->type == CELL_DATA_TYPE_FLOAT) {
        char* node_as_char_ptr = (char*) malloc(sizeof(char)*(int) std::ceil(std::log10(node->value._float)));
        sprintf(node_as_char_ptr, "%f", node->value._float);
        node_as_string.assign(node_as_char_ptr);
    }

    std::map<std::string, int>::iterator it = node_map.find(node_as_string);

    if (it != node_map.end()) {
        return it->second;
    } else {
        int new_node = next_node_int();
        node_map.insert(std::make_pair(node_as_string, new_node));
        return new_node;
    }
}