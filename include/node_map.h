/**
 * Helper file to map nodes to ints
 */

#ifndef PEPPER_NODE_MAP_H
#define PEPPER_NODE_MAP_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "table.h"

EXTERNC int node_to_int(data_t* node);

#undef EXTERNC
#endif //PEPPER_NODE_MAP_H
