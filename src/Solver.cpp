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
    os << "\nSolver {";
    os << "\n\tp (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.p[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tHperm (" << manager.size << "x" << manager.size << ") = \n";
    for (int i = 0; i < manager.size; ++i) {
        os << "\t[ ";
        for (int j = 0; j < manager.size; ++j) {
            os << std::setw(15) << std::left << manager.H[manager.p[i]][j] << ((j + 1 < manager.size) ? " " : "");
        }
        os << " ]\n";
    }
    os << "\n\tBperm (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.b[manager.p[j]] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tXperm (" << manager.size << "x1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << std::setw(5) << std::left << manager.x[manager.p[j]] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "}\n";
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
        printf("p[%d] = %d\n", j, p[j]);
    }
    printf("\n\n");
    for (int j = 0; j < size; ++j) {
        printf("x[p[%d]] = %lf\n", j, x[p[j]]);
    }
}

void Solver::LUGEPP(Matrix *A) {
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

void Solver::forwardSubstitution(Matrix *L, Vector *y, Vector *z) {
    for (int k = 0; k < size; ++k) {
        (*y)[p[k]] = (*z)[p[k]];
        for (int j = 0; j <= k - 1; ++j) {
            (*y)[p[k]] -= (*L)[p[k]][j] * (*y)[p[j]];
        }
        //(*y)[p[k]] /= (*L)[p[k]][k]; commented out because L matrix in our case has the diagonal shared with the U matrix
    }
}

void Solver::backwardSubstitution(Matrix *U, Vector *x, Vector *y) {
    for (int k = (int) size - 1; k >= 0; --k) {
        (*x)[p[k]] = (*y)[p[k]];
        for (int j = k + 1; j < size; ++j) {
            (*x)[p[k]] -= (*U)[p[k]][j] * (*x)[p[j]];
        }
        (*x)[p[k]] /= (*U)[p[k]][k];
    }
}

void Solver::solve() {
    LUGEPP(&H);
    Vector y = Vector(size);
    forwardSubstitution(&H, &y, &b);
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
    double maxHik = H[p[k]][k];
    for (int i = k; i < size; ++i) {
        if (p[i] > p[k]) {
            double hik = H[p[i]][k];
            if (hik > maxHik) {
                maxHik = hik;
                line = i;
            }
        }
    }
    return line;
}

void Solver::permutate(int line1, int line2) {
    if (line1 != line2) {
        int swap = p[line1];
        p[line1] = line2;
        p[line2] = swap;
    }
}

void Solver::stamp(std::vector<Component*> components) {
    for (auto &component : components) {
        component->stamp(this);
    }
}
