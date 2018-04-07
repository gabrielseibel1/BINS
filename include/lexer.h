/**
 * Defines function headers for parsing CSV files
 */

#ifndef C_CLUSTERING_CSV_PARSER_H
#define C_CLUSTERING_CSV_PARSER_H

#include "../include/table.h"

table_t* lex(char *filename);
void to_upper_case(char* string);
void lex_test();

#endif //C_CLUSTERING_CSV_PARSER_H
