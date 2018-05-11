#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

#include <vector>
#include "table.h"

class MatrixManager {
    using Matrix = std::vector<std::vector<double> >;
    using Vector = std::vector<double>;

private:
    Matrix H;
    std::vector<double> x;
    std::vector<double> b;
    size_t size;


public:
    MatrixManager(size_t size);

    virtual ~MatrixManager();

    size_t getSize() const;

    friend std::ostream &operator<<(std::ostream &os, const MatrixManager &manager);

    void stamp(component_t component);

    void requiredPrint();
};
#endif //BINS_MATRIX_MANAGER_H
