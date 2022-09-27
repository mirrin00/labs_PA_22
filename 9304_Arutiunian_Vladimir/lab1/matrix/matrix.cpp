#include "matrix.h"

Matrix::Matrix(const int height, const int width) : matrix_(
        std::vector<std::vector<int>>(height, std::vector<int>(width))),
                                                    width_(width),
                                                    height_(height) {}

int Matrix::getWidth() const {
    return width_;
}

int Matrix::getHeight() const {
    return height_;
}

int Matrix::get(int height, int width) const {
    return matrix_[height][width];
}

int Matrix::set(int height, int width, int value) {
    return matrix_[height][width] = value;
}

int* Matrix::writeByPointer(int* pointer) const {
    int height = getHeight();
    int width = getWidth();

    write(pointer, height);
    ++pointer;

    write(pointer, width);
    ++pointer;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            write(pointer, matrix_[i][j]);
            ++pointer;
        }
    }

    return pointer;
}

int* Matrix::write(int* pointer, int number) const {
    *pointer = number;
    return pointer;
}

Matrix Matrix::operator+(const Matrix& matrix) const {
    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.getHeight(); ++i) {
        for (int j = 0; j < newMatrix.getWidth(); ++j) {
            newMatrix.matrix_[i][j] += matrix.get(i, j);
        }
    }

    return newMatrix;
}

Matrix Matrix::parallelSum(const Matrix& matrix, const int threads_count) const {
    int dimension = matrix.getWidth() * matrix.getHeight();

    if (threads_count <= 0 || threads_count >= dimension) {
        throw std::runtime_error("Invalid threads count");
    }

    int elements_count_per_thread = (dimension + threads_count - 1) / threads_count + 1;

    auto summed_matrix = Matrix(height_, width_);

    std::vector<std::thread> threads;
    threads.reserve(threads_count);

    for (int i = 0; i < dimension; i += elements_count_per_thread) {
        std::thread aloneThread(sumSubVector, std::ref(summed_matrix), std::ref(*this), std::ref(matrix), i, elements_count_per_thread);
        threads.push_back(std::move(aloneThread));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return summed_matrix;
}

void sumSubVector(Matrix& summed_matrix, const Matrix& first, const Matrix& second, int begin_index, int length) {
    int dimension = summed_matrix.getWidth() * summed_matrix.getHeight();
    for (int i = begin_index; i < begin_index + length && i < dimension; ++i) {
        int height = i / summed_matrix.getWidth();
        int width = i % summed_matrix.getWidth();
        int sum = first.get(height, width) + second.get(height, width);
        summed_matrix.set(height, width, sum);
    }
}

std::pair<Matrix, int*> Matrix::readByPointer(int* pointer) {

    int width = *pointer;
    ++pointer;

    int height = *pointer;
    ++pointer;

    auto newMatrix = Matrix(width, height);

    for (int i = 0; i < newMatrix.getHeight(); ++i) {
        for (int j = 0; j < newMatrix.getWidth(); ++j) {
            newMatrix.set(i, j, *pointer);
            ++pointer;
        }
    }

    return std::make_pair(newMatrix, pointer);
}

std::istream& operator>>(std::istream& in, Matrix& matrix) {

    in >> matrix.height_;
    in >> matrix.width_;

    int height = matrix.getHeight();
    int width = matrix.getWidth();

    matrix.matrix_.resize(height);

    for (int i = 0; i < height; ++i) {
        matrix.matrix_[i].resize(width);
        for (int j = 0; j < width; ++j) {
            in >> matrix.matrix_[i][j];
        }
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    int height = matrix.getHeight();
    int width = matrix.getWidth();

    out << height << ' ';
    out << width << '\n';

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            out << matrix.get(i, j);
            if (j < width - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}

