//
// Created by gabriel on 7/8/18.
//

#ifndef BINS_TRANSOLVER_H
#define BINS_TRANSOLVER_H

class DynamicComponent;
class SpiceInterpreter;

#include "../include/OPSolver.h"

#define STD_STEP 1E-3
#define STD_STOP 1E3

class TranSolver {
    using DoubleVector = std::vector<double>;

private:
    SpiceInterpreter *spiceInterpreter;
    OPSolver *opSolver;
    double maxTime;
    double step;

    size_t size;

    size_t nodeCount;

    size_t group2Size;

    std::vector<Component *> staticComponents;

    std::vector<DynamicComponent *> dynamicComponents;

    /**
     * Vector of pairs <time, X>, where X is a solution vector
     */
    std::vector<std::pair<double, DoubleVector>> solutions;
public:

    explicit TranSolver(SpiceInterpreter *interpreter);

    virtual ~TranSolver();

    void solveTransient();

    void updateDynamicComponents(double time, OPSolver *opSolver);

    void printSolutions(char *filename);
};


#endif //BINS_TRANSOLVER_H
