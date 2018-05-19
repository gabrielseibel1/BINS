/**
 * Defines function headers for parsing CSV files
 */

#ifndef BINS_SPICE_READER_H
#define BINS_SPICE_READER_H

#include "table.h"

class SpiceReader {

private:
    char *filename;
    table_t *spiceTable;

    void to_upper_case(char *string);

public:
    explicit SpiceReader(char *filename);

    virtual ~SpiceReader();

    void read();

    table_t *getSpiceTable() const;
};

#endif //BINS_SPICE_READER_H
