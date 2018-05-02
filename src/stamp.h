//
// Created by gabriel on 5/2/18.
//

#include "../include/table.h"

#ifndef BINS_STAMP_H
#define BINS_STAMP_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/**
 * Stamps the matrix with a pattern defined by a provided component
 */
void stamp(double **matrix, component_t *component);

#undef EXTERNC
#endif //BINS_STAMP_H
