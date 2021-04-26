#ifndef MAIN_CPP_MATRIX_H
#define MAIN_CPP_MATRIX_H

#include <vector>

class Matrix {
private:
    std::vector<std::vector<double>> matrix;
public:
    explicit Matrix(std::vector<std::vector<double>> matrix);
    Matrix(size_t nRow, size_t nCol);
    Matrix() = default;
    size_t nCol();
    size_t nRow();
    double getValue(size_t rowIndex, size_t colIndex);
    void setValue(size_t rowIndex, size_t colIndex, double value);
    void displayMatrix();

    // https://stackoverflow.com/questions/55366456/how-to-make-kronecker-product-of-two-random-vectors-of-vectors
    std::vector<std::vector<double>> KroneckerProduct(Matrix v1);
};


#endif //MAIN_CPP_MATRIX_H
