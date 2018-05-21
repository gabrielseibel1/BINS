//
// Created by gabriel on 5/2/18.
//

#include <vector>
#include <ostream>
#include <iostream>
#include <iomanip>
#include "../include/Solver.h"

Solver::Solver(size_t size, size_t group1Size) : size(size), nodesCount(group1Size) {
    H = std::vector<std::vector<double> >(size);
    for (int i = 0; i < size; ++i) {
        H[i] = std::vector<double>(size);
    }
    x = std::vector<double>(size);
    b = std::vector<double>(size);
    p = std::vector<int>(size);
    for (int i = 0; i < size; ++i) {
        p[i] = i;
    }
}

Solver::~Solver() = default;

std::ostream &operator<<(std::ostream &os, const Solver &manager) {
    os << "\nMatrixManager {";
    os << "\n\tH (" << manager.size << "x" << manager.size << ") = \n";
    for (int i = 0; i < manager.size; ++i) {
        os << "\t[ ";
        for (int j = 0; j < manager.size; ++j) {
            os << std::setw(10) << std::left << manager.H[i][j] << ((j + 1 < manager.size) ? " " : "");
        }
        os << " ]\n";
    }
    os << "\n\tx (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << manager.x[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tb (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << manager.b[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tp (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << manager.p[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    return os;
}

void Solver::requiredPrint() {
    printf("\n\n");
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("H[%d][%d] = %lf\n", i, j, H[i][j]);
        }
    }
    printf("\n\n");
    for (int j = 0; j < size; ++j) {
        printf("x[%d] = %lf\n", j, x[j]);
    }
    printf("\n\n");
    for (int j = 0; j < size; ++j) {
        printf("b[%d] = %lf\n", j, b[j]);
    }
    printf("\n\n");
    for (int j = 0; j < size; ++j) {
        printf("p[%d] = %lf\n", j, b[j]);
    }
}

void Solver::LUGEPP(Matrix *A) {
    for (int k = 0; k < size; ++k) {
        //int pivotLine = lineWithLargestPivot(k);
        //permutate(k, pivotLine);

        for (int i = k + 1; i < size; ++i) {
            (*A)[i][k] /= (*A)[k][k];
            for (int j = k + 1; j < size; ++j) {
                (*A)[i][j] -= (*A)[i][k] * (*A)[k][j];
            }
        }
    }
}

void Solver::forwardSubstitution(Matrix *L, Vector *y, Vector *z) {
    for (int k = 0; k < size; ++k) {
        (*y)[k] = (*z)[k];
        for (int j = 0; j <= k - 1; ++j) {
            (*y)[k] -= (*L)[k][j] * (*y)[j];
        }
        //(*y)[k] /= (*L)[k][k]; commented out because L matrix in our case has the diagonal shared with the U matrix
    }
}

void Solver::backwardSubstitution(Matrix *U, Vector *x, Vector *y) {
    for (int k = (int) size - 1; k >= 0; --k) {
        (*x)[k] = (*y)[k];
        for (int j = k + 1; j < size; ++j) {
            (*x)[k] -= (*U)[k][j] * (*x)[j];
        }
        (*x)[k] /= (*U)[k][k];
    }
}

void Solver::solve() {
    LUGEPP(&H);
    Vector y = Vector(size);
    forwardSubstitution(&H, &y, &b);
    std::cout << "y = "; for (auto el : y) std::cout << el << ",";
    backwardSubstitution(&H, &x, &y);
}

void Solver::buildMatricesFromStdIn() {
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

size_t Solver::getSize() const {
    return size;
}

int Solver::lineWithLargestPivot(int k) {
    int line = k;
    double maxHik = HP(k,k);
    for (int i = k; i < size; ++i) {
        double hik = HP(i,k);
        if (hik > maxHik) {
            maxHik = hik;
            line = i;
        }
    }
    std::cout << "largest pivot from " << k << " is in line " << line << "\n";
    return line;
}

void Solver::permutate(int line1, int line2) {
    if (line1 != line2) {
        int pLine1 = p[line1];
        int pLine2 = p[line2];

        p[pLine1] = pLine1;
        p[pLine2] = pLine1;
        std::cout << "p[" << pLine1 << "] = " << pLine2 << "\n";
        std::cout << "p[" << pLine2 << "] = " << pLine1 << "\n";
    }
}

void Solver::stamp(std::vector<Component*> components) {
    for (auto &component : components) {
        component->stamp(this);
        std::cout << *this;
    }
}
