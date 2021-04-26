#include "Matrix.h"
#include <iostream>

Matrix::Matrix(std::vector<std::vector<double>> matrix) {
    this->matrix = matrix;
}

Matrix::Matrix(size_t nRow, size_t nCol) {
    this->matrix = std::vector<std::vector<double>>(nRow, std::vector<double>(nCol));
}

size_t Matrix::nCol() {
    return matrix[0].size();
}

size_t Matrix::nRow() {
    return matrix.size();
}

double Matrix::getValue(size_t rowIndex, size_t colIndex) {
    return matrix[rowIndex][colIndex];
}

void Matrix::setValue(size_t rowIndex, size_t colIndex, double value) {
    matrix[rowIndex][colIndex] = value;
}

std::vector<std::vector<double>> Matrix::KroneckerProduct(Matrix v2) {
    std::vector<std::vector<double>> v(matrix.size() * v2.nRow(), std::vector<double>(matrix[0].size() * v2.nCol()));
    for (std::size_t z1(0); z1 < matrix.size(); ++z1) {
        for (std::size_t z2(0); z2 < v2.nRow(); ++z2) {
            for (std::size_t z3(0); z3 < matrix[0].size(); ++z3) {
                for (std::size_t z4(0); z4 < v2.nCol(); ++z4) {
                    v[z1*v2.nRow() + z2][z3*v2.nCol() + z4] = matrix[z1][z3] * v2.getValue(z2, z4);
                }
            }
        }
    }
    return v;
}

void Matrix::displayMatrix() {
    for (const auto& row: matrix) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << '\n';
    }
    std::cout << "\n";
}