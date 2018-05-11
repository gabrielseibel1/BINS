//
// Created by gabriel on 5/2/18.
//

#include <vector>
#include <ostream>
#include <iostream>
#include "../include/MatrixManager.h"


MatrixManager::MatrixManager(size_t size) : size(size) {
    H = std::vector<std::vector<double> >(size);
    for (int i = 0; i < size; ++i) {
        H[i] = std::vector<double>(size);
    }
    x = std::vector<double>(size);
    b = std::vector<double>(size);
}

MatrixManager::~MatrixManager() = default;

std::ostream &operator<<(std::ostream &os, const MatrixManager &manager) {
    os << "\nMatrixManager {";
    os << "\n\tH (" << manager.size << ", " << manager.size << ") = \n";
    for (int i = 0; i < manager.size; ++i) {
        os << "\t[ ";
        for (int j = 0; j < manager.size; ++j) {
            os << manager.H[i][j] << ((j + 1 < manager.size) ? " " : "");
        }
        os << " ]\n";
    }
    os << "\n\tx (" << manager.size << ", 1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << manager.x[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    os << "\n\tb (" << manager.size << ", 1) = \n\t[ ";
    for (int j = 0; j < manager.size; ++j) {
        os << manager.b[j] << ((j + 1 < manager.size) ? " ]\n\t[ " : "");
    }
    os << " ]\n";
    return os;
}

void MatrixManager::requiredPrint() {
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
}

void MatrixManager::stamp(component_t component) {
    int index = component.id;
    switch (component.type) {
    }
}

size_t MatrixManager::getSize() const {
    return size;
}
