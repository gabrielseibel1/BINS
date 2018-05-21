#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

class Component;

#include <vector>
#include "Component.h"


#define HP(i, j) H [p[i]] [j]
#define xP(i) x [p[i]]
#define bP(i) b [p[i]]

class Solver {
    using Matrix = std::vector<std::vector<double> >;
    using Vector = std::vector<double>;

private:
    void permutate(int k, int line);

    int lineWithLargestPivot(int k);

    void backwardSubstitution(Matrix *U, Vector *x, Vector *y);

    void forwardSubstitution(Matrix *L, Vector *y, Vector *z);

    void LUGEPP(Matrix *A);

public:
    Matrix H;
    Vector x;
    Vector b;
    std::vector<int> p;
    size_t size;
    size_t nodesCount;

    explicit Solver(size_t size, size_t group2Size);

    virtual ~Solver();

    size_t getSize() const;

    friend std::ostream &operator<<(std::ostream &os, const Solver &manager);

    void requiredPrint();

    void buildMatricesFromStdIn();

    void solve();

    void stamp(std::vector<Component*> components);
};
#endif //BINS_MATRIX_MANAGER_H
