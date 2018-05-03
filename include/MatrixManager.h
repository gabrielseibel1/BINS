#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

#include <vector>

class MatrixManager {
    using Matrix = std::vector<std::vector<double> >;

private:
    Matrix matrix;
    size_t nRows;
    size_t nCols;

public:
    MatrixManager(size_t n_rows, size_t n_cols);

    virtual ~MatrixManager();

    size_t getNRows() const;

    size_t getNCols() const;

    friend std::ostream &operator<<(std::ostream &os, const MatrixManager &manager);
};
#endif //BINS_MATRIX_MANAGER_H
