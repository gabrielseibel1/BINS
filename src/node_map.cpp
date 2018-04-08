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
#include <iostream>

std::map<std::string, int> node_map;
int last_node_int;

int next_node_int() {
    return ++last_node_int;
}

int node_to_int(data_t* node) {

    std::string node_as_string;

    if (node->type == CELL_DATA_TYPE_STRING) {
        node_as_string.assign(node->value._string);
    } else if (node->type == CELL_DATA_TYPE_DOUBLE) {
        std::ostringstream buff;
        buff << node->value._double;
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

void print_node_map() {
    std::cout << "\n\nNODE MAP:\n";

    std::map<std::string, int>::iterator it = node_map.begin();
    while (it != node_map.end()) {
        std::cout << "ID: " << it->second << "\t\tLABEL: " << it->first << "\n";
        ++it;
    }
}

void clear_node_map() {
    std::map<std::string, int>::iterator it = node_map.begin();
    node_map.clear();
}