#pragma once

#include <fstream>
#include <vector>
#include <mutex>
#include <condition_variable>



class Matrix {
public:

    enum class Status {
        generate,
        sum,
        output
    };

    explicit Matrix(const unsigned& row, const unsigned& column)
        : rowSize(row), colSize(column), size(rowSize * colSize)
        {
            firstMatrix.resize(size);
            secondMatrix.resize(size);
            result.resize(size);
            status = Status::generate;
        }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
    {
        for (unsigned j = 0; j < matrix.colSize; ++j)
        {
            for (unsigned i = j; i < matrix.size; i += matrix.colSize)
                out << matrix.result[i] << ' ';

            out << '\n';
        }
            
        return out;
    }

    const unsigned rowSize;
    const unsigned colSize;
    const unsigned size;
    std::vector<int> firstMatrix;
    std::vector<int> secondMatrix;
    std::vector<int> result;
    std::condition_variable threadBell;
    std::mutex matrixMutex;
    Status status;
};
    