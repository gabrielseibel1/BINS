/**
 * Helper file to map nodes to ints
 */

#include "../include/node_map.h"
#include <cmath>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>

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
        std::ostringstream buff;
        buff << node->value._float;
        node_as_string = buff.str();
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

void clear_node_map() {
    std::map<std::string, int>::iterator it = node_map.begin();
    node_map.clear();
}