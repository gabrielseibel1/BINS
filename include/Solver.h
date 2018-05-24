#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

class Component;

#include <vector>
#include "Component.h"

#define ABSTOL 1E-6
#define RELTOL 1E-3
#define MAX_ITER_REFINEMENT 500

class Solver {
    using DoubleMatrix = std::vector<std::vector<double> >;
    using DoubleVector = std::vector<double>;
    using LongDoubleMatrix = std::vector<std::vector<long double> >;
    using LongDoubleVector = std::vector<long double>;

private:
    void permutate(int k, int line);

    int lineWithLargestPivot(int k);

    void backwardSubstitution(DoubleMatrix *U, DoubleVector *x, DoubleVector *y);

    void forwardSubstitution(DoubleMatrix *L, DoubleVector *y, DoubleVector *z);

    void LUGEPP(DoubleMatrix *A);

    void forwardSubstitution(DoubleMatrix *L, DoubleVector *y, LongDoubleVector *z);

public:
    DoubleMatrix H;
    DoubleVector x;
    DoubleVector b;
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

    void iterativeRefinement(const LongDoubleMatrix &vector);

    double norm(DoubleVector vector);

    void stamp(std::vector<Component*> components);

    void saveOriginalMatrix(LongDoubleMatrix *A);

    void measureRefinementChanges(const DoubleVector &xBeforeRefinement) const;
};
#endif //BINS_MATRIX_MANAGER_H
