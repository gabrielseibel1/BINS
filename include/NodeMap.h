/**
 * Helper file to map nodes to ints
 */

#ifndef BINS_NODE_MAP_H
#define BINS_NODE_MAP_H

#include <string>
#include <map>
#include <ostream>
#include "table.h"

class NodeMap {
private:
    std::map<std::string, int> nodeMap;
    int lastNodeInt = 0;

    int next_node_int();

public:
    size_t getSize();

    int nodeToInt(data_t *node);

    friend std::ostream &operator<<(std::ostream &os, const NodeMap &map);
};


#endif //BINS_NODE_MAP_H
