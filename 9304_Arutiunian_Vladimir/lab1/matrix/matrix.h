#ifndef LABS_PA_22_MATRIX_H
#define LABS_PA_22_MATRIX_H

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>

class Matrix {
private:
    std::vector<std::vector<int>> matrix_;
    int width_;
    int height_;

    int* write(int* pointer, int number) const;

public:
    Matrix() = default;

    Matrix(const Matrix& matrix) = default;

    Matrix(const int height, const int width);

    ~Matrix() = default;

    int getWidth() const;

    int getHeight() const;

    int get(int height, int width) const;

    int set(int height, int width, int value);

    int* writeByPointer(int* pointer) const;

    Matrix operator+(const Matrix& matrix) const;

    Matrix parallelSum(const Matrix& matrix, const int threads_count) const;

    static std::pair<Matrix, int*> readByPointer(int* pointer);

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
};

void sumSubVector(Matrix& summed_matrix, const Matrix& first, const Matrix& second, int begin_index, int length);

#endif //LABS_PA_22_MATRIX_H
