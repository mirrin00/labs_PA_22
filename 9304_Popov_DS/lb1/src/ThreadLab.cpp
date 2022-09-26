#include <random>
#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "matrix.h"

#define ROW 1000
#define COL 1000

// #include <chrono>
// #include <iostream>


void generateMatrix(Matrix* const matrix);
void sumMatrix(Matrix* const matrix);
void resultMatrix(Matrix* const matrix);



void startLabInThreadMode()
{
    Matrix* matrix = new Matrix(ROW, COL);

    std::thread firstThread (generateMatrix, matrix);
    std::thread secondThread(sumMatrix,      matrix);
    std::thread thirdThread (resultMatrix,   matrix);

    firstThread.join();
    secondThread.join();
    thirdThread.join();

    matrix->threadBell.notify_all();

    delete matrix;
}


void generateMatrix(Matrix* const matrix)
{
    std::unique_lock<std::mutex> ul(matrix->matrixMutex);
    matrix->threadBell.wait(ul, [=](){ return matrix->status == Matrix::Status::generate; });

    for (unsigned i = 0; i < matrix->size; ++i)
    {
        matrix->firstMatrix [i] = rand() % 28 + 41;
        matrix->secondMatrix[i] = rand() % 28 + 1;
    }

    matrix->status = Matrix::Status::sum;

    matrix->threadBell.notify_all();
}


void sumMatrix(Matrix* const matrix)
{
    std::unique_lock<std::mutex> ul(matrix->matrixMutex);
    matrix->threadBell.wait(ul, [=](){ return matrix->status == Matrix::Status::sum; });

    // auto begin = std::chrono::steady_clock::now();

    for (unsigned i = 0; i < matrix->size; ++i)
        matrix->result[i] = matrix->firstMatrix[i] + matrix->secondMatrix[i];

    // auto sum = [matrix](unsigned index, unsigned size)
    // {
    //     for (unsigned i = index; i < index + size; ++i)
    //         matrix->result[i] = matrix->firstMatrix[i] + matrix->secondMatrix[i];
    // };

    // unsigned f = matrix->size / 5;
    // unsigned e = matrix->size % 5;

    // std::vector<std::thread> pullThread;

    // for (unsigned i = 0; i < 4; ++i)
    //     pullThread.emplace_back(sum, i * f, f);

    // pullThread.emplace_back(sum, matrix->size - (f + e), f + e);

    // for (auto& t : pullThread)
    //     t.join();

    // auto end = std::chrono::steady_clock::now();
  
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    // std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    matrix->status = Matrix::Status::output;

    matrix->threadBell.notify_all();
}


void resultMatrix(Matrix* const matrix)
{
    std::unique_lock<std::mutex> ul(matrix->matrixMutex);
    matrix->threadBell.wait(ul, [=](){ return matrix->status == Matrix::Status::output; });

    std::ofstream resultFile("./result.txt");

    if (resultFile)
    {
        resultFile << *matrix;
        resultFile.close();
    }
}