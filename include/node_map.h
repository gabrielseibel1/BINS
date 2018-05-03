/**
 * Helper file to map nodes to ints
 */

#ifndef BINS_NODE_MAP_H
#define BINS_NODE_MAP_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "table.h"

EXTERNC int node_to_int(data_t* node);
EXTERNC void print_node_map();
EXTERNC void clear_node_map();

#undef EXTERNC
#endif //BINS_NODE_MAP_H
