#ifndef MATRIX_H
#define MATRIX_H
#include <array>
#include <vector>

class Matrix
{
public:
    std::vector<double> matrix;
    int n;
    int m;
    Matrix();
    Matrix(int32_t n, int32_t m);
    Matrix(std::vector<double> matrix, int32_t n, int32_t m);
    Matrix(std::vector<double> threeSizedMatrix);
    Matrix mult(Matrix &other);
    Matrix mult(double scalar);
    Matrix sub(Matrix &other);
};

#endif // MATRIX_H
