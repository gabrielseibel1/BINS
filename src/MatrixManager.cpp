//
// Created by gabriel on 5/2/18.
//

#include <vector>
#include <ostream>
#include "../include/MatrixManager.h"


MatrixManager::MatrixManager(size_t n_rows, size_t n_cols) : nRows(n_rows), nCols(n_cols) {
    matrix = std::vector<std::vector<double> >(n_rows);
    for (int i = 0; i < n_rows; ++i) {
        matrix[i] = std::vector<double>(n_cols);
    }
}

size_t MatrixManager::getNRows() const {
    return nRows;
}

size_t MatrixManager::getNCols() const {
    return nCols;
}

std::ostream &operator<<(std::ostream &os, const MatrixManager &manager) {
    os << "MatrixManager {";
    os << "\n\tnRows: " << manager.nRows;
    os << "\n\tnCols: " << manager.nCols;
    os << "\n\tmatrix: \n";
    for (int i = 0; i < manager.nRows; ++i) {
        os << "\t[ ";
        for (int j = 0; j < manager.nCols; ++j) {
            os << manager.matrix[i][j] << ((j + 1  < manager.nCols) ? " " : "");
        }
        os << " ]\n";
    }
    os << "}\n";
    return os;
}

MatrixManager::~MatrixManager() = default;

