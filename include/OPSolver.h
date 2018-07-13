#ifndef BINS_MATRIX_MANAGER_H
#define BINS_MATRIX_MANAGER_H

class Component;
class DynamicComponent;
class SpiceInterpreter;

#include <vector>
#include <ostream>

#define ABSTOL 1E-6
#define RELTOL 1E-3
#define MAX_ITER_REFINEMENT 500

class OPSolver {
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

    explicit OPSolver(size_t size, size_t group1Size);

    virtual ~OPSolver();

    int getSize() const;

    friend std::ostream &operator<<(std::ostream &os, const OPSolver &manager);

    void buildMatricesFromStdIn();

    void solveOP();

    void iterativeRefinement(const LongDoubleMatrix &vector);

    double norm(DoubleVector vector);

    void accumulativeStamp(std::vector<Component *> components);

    void substitutiveStamp(std::vector<DynamicComponent *> components, double step, double time);

    void saveOriginalMatrix(LongDoubleMatrix *A);

    void measureRefinementChanges(const DoubleVector &xBeforeRefinement) const;

    void interpretedPrint(SpiceInterpreter *interpreter);

    void sum(OPSolver *pSolver);

    void sub(OPSolver *pSolver);
};
#endif //BINS_MATRIX_MANAGER_H
