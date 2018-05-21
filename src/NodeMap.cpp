/**
 * Helper file to map nodes to ints
 */

#include <cmath>
#include <sstream>
#include <iostream>
#include "../include/NodeMap.h"

int NodeMap::next_node_int() {
    return ++lastNodeInt;
}

int NodeMap::nodeToInt(data_t *node) {

    if (nodeMap.empty()) nodeMap.insert(std::make_pair("0", 0));

    std::string node_as_string;

    if (node->type == CELL_DATA_TYPE_STRING) {
        node_as_string.assign(node->value._string);
    } else if (node->type == CELL_DATA_TYPE_DOUBLE) {
        std::ostringstream buff;
        buff << node->value._double;
        node_as_string = buff.str();
    }

    auto it = nodeMap.find(node_as_string);

    if (it != nodeMap.end()) {
        return it->second;
    } else {
        int new_node = next_node_int();
        nodeMap.insert(std::make_pair(node_as_string, new_node));
        return new_node;
    }
}

std::ostream &operator<<(std::ostream &os, const NodeMap &map) {
    std::cout << "\n\nNODE MAP:\n";

    for (auto pair : map.nodeMap) {
        std::cout << "ID: " << pair.second << "\t\tLABEL: " << pair.first << "\n";
    }
}

size_t NodeMap::getSize() {
    return nodeMap.size();
}
