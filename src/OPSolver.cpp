//
// Created by gabriel on 5/2/18.
//

#include <vector>
#include <ostream>
#include <iostream>
#include <iomanip>
#include "../include/Component.h"
#include "../include/SpiceInterpreter.h"
#include "../include/OPSolver.h"

OPSolver::OPSolver(size_t size, size_t group1Size) : size(size), nodesCount(group1Size) {
    H = DoubleMatrix(size);
    for (int i = 0; i < size; ++i) {
        H[i] = std::vector<double>(size);
    }
    x = DoubleVector(size);
    b = DoubleVector(size);
    p = std::vector<int>(size);
    for (int i = 0; i < size; ++i) {
        p[i] = i;
    }
}

OPSolver::~OPSolver() = default;

std::ostream &operator<<(std::ostream &os, const OPSolver &manager) {
    os << "\nSOLVER: {\n";
    os << "\n\tP (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.p[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tH_PERM (" << manager.size << "x" << manager.size << ") = \n";
    for (int i = 0; i < manager.size; ++i) {
        os << "\t[ ";
        for (int j = 0; j < manager.size; ++j) {
            os << std::setw(15) << std::left << manager.H[manager.p[i]][j] << ((j + 1 < manager.size) ? " " : "");
        }
        os << " ]\n";
    }
    os << "\n\tB_PERM (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.b[manager.p[j]] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tX_PERM (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.x[manager.p[j]] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tX (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.x[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n\n";
    os << "}\n";
    return os;
}

void OPSolver::LUGEPP(DoubleMatrix *A) {
    for (int k = 0; k < size; ++k) {
        int pivotLine = lineWithLargestPivot(k);
        permutate(k, pivotLine);

        for (int i = k + 1; i < size; ++i) {
            (*A)[p[i]][k] /= (*A)[p[k]][k];
            for (int j = k + 1; j < size; ++j) {
                (*A)[p[i]][j] -= (*A)[p[i]][k] * (*A)[p[k]][j];
            }
        }
    }
}

void OPSolver::forwardSubstitution(DoubleMatrix *L, DoubleVector *y, DoubleVector *z) {
    for (int k = 0; k < size; ++k) {
        (*y)[p[k]] = (*z)[p[k]];
        for (int j = 0; j <= k - 1; ++j) {
            (*y)[p[k]] -= (*L)[p[k]][j] * (*y)[p[j]];
        }
        //(*y)[p[k]] /= (*L)[p[k]][k]; commented out because L matrix in our case has the diagonal shared with the U matrix
    }
}

void OPSolver::forwardSubstitution(DoubleMatrix *L, DoubleVector *y, LongDoubleVector *z) {
    for (int k = 0; k < size; ++k) {
        (*y)[p[k]] = (*z)[p[k]];
        for (int j = 0; j <= k - 1; ++j) {
            (*y)[p[k]] -= (*L)[p[k]][j] * (*y)[p[j]];
        }
        //(*y)[p[k]] /= (*L)[p[k]][k]; commented out because L matrix in our case has the diagonal shared with the U matrix
    }
}

void OPSolver::backwardSubstitution(DoubleMatrix *U, DoubleVector *x, DoubleVector *y) {
    for (int k = (int) size - 1; k >= 0; --k) {
        (*x)[p[k]] = (*y)[p[k]];
        for (int j = k + 1; j < size; ++j) {
            (*x)[p[k]] -= (*U)[p[k]][j] * (*x)[p[j]];
        }
        (*x)[p[k]] /= (*U)[p[k]][k];
    }
}

void OPSolver::solveOP() {
    LongDoubleMatrix A = LongDoubleMatrix(size);
    for (int i = 0; i < size; ++i) {
        A[i] = std::vector<long double>(size);
    }
    saveOriginalMatrix(&A);

    LUGEPP(&H);
    DoubleVector y = DoubleVector(size);
    forwardSubstitution(&H, &y, &b);
    backwardSubstitution(&H, &x, &y);

    //copy x vector for later comparison
    DoubleVector xBeforeRefinement = DoubleVector(size);
    for (int i = 0; i < size; ++i) {
        xBeforeRefinement[i] = x[i];
    }

    iterativeRefinement(A);

    //measureRefinementChanges(xBeforeRefinement);
}

void OPSolver::measureRefinementChanges(const OPSolver::DoubleVector &xBeforeRefinement) const {
    //compare x before and after iterative refinement
    LongDoubleVector difference = LongDoubleVector(size);
    for (int i = 0; i < size; ++i) {
        difference[i] = xBeforeRefinement[i] - x[i];
    }
    std::cout << "Difference between X before and after iterative refinement: \n";
    std::cout << "XDif (" << size << "x1) = \n\t[ ";
    for (int j = 0; j < size; ++j) {
        std::cout << std::setw(15) << std::left << difference[p[j]] << ((j + 1 < size) ? " ]\n\t[ " : "");
    }
    std::cout << " ]\n";
}

void OPSolver::iterativeRefinement(const LongDoubleMatrix &A) {
    /*std::cout << "Running iterative refinement "
            "(ABSTOL = " << ABSTOL << ", RELTOL = " << RELTOL << ", MAX_ITER = " << MAX_ITER_REFINEMENT << ") ...\n\n";*/

    int iteration = 0;
    //iterative refinement
    DoubleVector z;
    do {
        //solve Ax with increased precision
        LongDoubleVector Ax = LongDoubleVector(size);
        for (int i = 0; i < size; ++i) {
            long double sum = 0;
            for (int j = 0; j < size; ++j) {
                sum += A[p[i]][j] * x[p[j]];
            }
            Ax[p[i]] = sum;
        }

        //create residue vector with increased precision
        LongDoubleVector r = LongDoubleVector(size);
        for (int i = 0; i < size; ++i) {
            r[i] = b[i] - Ax[i];
        }

        //solve LUz = Pr (Hz = Pr)
        z = DoubleVector(size);
        DoubleVector w = DoubleVector(size);
        forwardSubstitution(&H, &w, &r);
        backwardSubstitution(&H, &z, &w);

        //x = x + z
        for (int i = 0; i < size; ++i) {
            x[i] += z[i];
        }

        iteration++;
    }
    while ((norm(z) <= RELTOL * norm(x) + ABSTOL) && (iteration <= MAX_ITER_REFINEMENT));
}

void OPSolver::buildMatricesFromStdIn() {
    std::cout << "Size? ";
    std::cin >> size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("H[%d][%d] = ", i, j);
            std::cin >> H[i][j];
        }
    }
    for (int j = 0; j < size; ++j) {
        printf("b[%d] = ", j);
        std::cin >> b[j];
    }
}

int OPSolver::getSize() const {
    return size;
}

int OPSolver::lineWithLargestPivot(int k) {
    int line = k;
    double maxHik = H[p[k]][k];
    for (int i = k; i < size; ++i) {
        double hik = H[p[i]][k];
        if (hik > maxHik) {
            maxHik = hik;
            line = i;
        }
    }
    return line;
}

void OPSolver::permutate(int line1, int line2) {
    if (line1 != line2) {
        int swap = p[line1];
        p[line1] = p[line2];
        p[line2] = swap;
    }
}

double OPSolver::norm(OPSolver::DoubleVector vector) {
    //find line with largest value
    double max = vector[p[0]];
    for (int i = 0; i < size; ++i) {
        double hik = vector[i];
        if (hik > max) max = hik;
    }
    return max;
}

void OPSolver::saveOriginalMatrix(OPSolver::LongDoubleMatrix *A) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            (*A)[i][j] = H[i][j];
        }
    }
}

void OPSolver::accumulativeStamp(std::vector<Component *> components) {
    for (Component *component : components) {
        component->stamp(this);
    }
}

void OPSolver::substitutiveStamp(std::vector<DynamicComponent *> dynamicComponents, double step, double time) {
    for (DynamicComponent *component : dynamicComponents) {
        component->hardStamp(this, step, time);
    }
}

void OPSolver::interpretedPrint(SpiceInterpreter *interpreter) {
    std::cout << "\n\nOPERATION POINT:\n";

    std::string varType = "V";
    NodeMap *nodeMap = interpreter->getNodeMap();
    for (auto pair: nodeMap->getNodeMap()) {
        if (pair.second == 0) continue;

        double value = x[p[pair.second - 1]];
        std::cout << varType << "(" << pair.first << ") = " << value << "\n";
    }

    varType = "I";
    for (Component *comp : interpreter->getComponents()) {
        if (comp->group == GROUP2){
            int index = static_cast<int>(nodeMap->getSize() - 1 + comp->indexInGroup);
            double value = x[p[index]];
            std::cout << varType << "(" << comp->label << ") = " << value << "\n";
        }
    }
    std::cout << "\n\n";
}

void OPSolver::sum(OPSolver *pSolver) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            H[i][j] += pSolver->H[i][j];
        }
    }
    for (int i = 0; i < size; ++i) {
        b[i] += pSolver->b[i];
    }
}

void OPSolver::sub(OPSolver *pSolver) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            H[i][j] -= pSolver->H[i][j];
        }
    }
    for (int i = 0; i < size; ++i) {
        b[i] -= pSolver->b[i];
    }
}
