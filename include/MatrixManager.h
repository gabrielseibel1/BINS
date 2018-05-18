#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

#include <vector>
#include "table.h"

#define HP(i, j) H [p[i]] [j]
#define xP(i) x [p[i]]
#define bP(i) b [p[i]]

class MatrixManager {
    using Matrix = std::vector<std::vector<double> >;
    using Vector = std::vector<double>;

private:
    Matrix H;
    Vector x;
    Vector b;
    std::vector<int> p;
    size_t size;


public:
    MatrixManager(size_t size);

    virtual ~MatrixManager();

    size_t getSize() const;

    friend std::ostream &operator<<(std::ostream &os, const MatrixManager &manager);

    void stamp(component_t component);

    void requiredPrint();

    int lineWithLargestPivot(int k);

    void permutate(int k, int line);

    void buildMatricesFromStdIn();

    void solve();

    void backwardSubstitution(Matrix *U, Vector *x, Vector *y);

    void forwardSubstitution(Matrix *L, Vector *y, Vector *z);

    void LUGEPP(Matrix *A);
};
#endif //BINS_MATRIX_MANAGER_H
