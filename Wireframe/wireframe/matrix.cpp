#include "matrix.h"

Matrix::Matrix()
{

}


Matrix::Matrix(int32_t n, int32_t m)
{
    this->n = n;
    this->m = m;
    this->matrix = std::vector<double>(m * n, 0);
}

Matrix::Matrix(std::vector<double> matrix, int32_t n, int32_t m)
{
    this->n = n;
    this->m = m;
    this->matrix = std::vector<double>(m * n, 0);
    for (int32_t i = 0; i < this->n; i++) {
        for (int32_t j = 0; j < this->m; j++) {
           this->matrix[i * this->m  + j] = 0.0;
        }
    }
}

Matrix::Matrix(std::vector<double> threeSizedMatrix) {
    this->n = 3;
    this->m = 3;
    matrix = threeSizedMatrix;
}

Matrix Matrix::mult(Matrix &other) {
    Matrix matrix = Matrix(this->n, other.m);
    for (int32_t i = 0; i < this->n; i++) {
        for (int32_t j = 0; j < other.m; j++) {
            for (int32_t k = 0; k < this->m; k++) {
                matrix.matrix[i * other.m + j] += this->matrix[i * m + k] * other.matrix[k * other.m + j];
            }
        }
    }
    return matrix;
}

Matrix Matrix::mult(double scalar) {
    Matrix matrix = Matrix(this->n, this->m);
    for (int32_t i = 0; i < this->n; i++) {
        for (int32_t j = 0; j < this->m; j++) {
            matrix.matrix[i * this->m + j] = this->matrix[i * this->m + j] * scalar;
        }
    }

    return matrix;
}

Matrix Matrix::sub(Matrix &other) {
    Matrix matrix = Matrix(this->n, this->m);
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->m; j++) {
            matrix.matrix[i * this->m + j] = this->matrix[i * this->m + j] - other.matrix[i * this->m + j];
        }
    }
    return matrix;
}


