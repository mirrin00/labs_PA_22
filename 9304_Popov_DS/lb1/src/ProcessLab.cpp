#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <random>
#include <unistd.h>
#include <fstream>


#include "ProcessLab.h"
#include "matrix.h"

#define ROW 1000
#define COL 1000



int* createSharedMemory(const unsigned size);
void generateMatrix(int* const shMem, const unsigned size);
void sumMatrix(int* const shMem, const unsigned size, int* const firstMatrix, int* const secondMatrix);
void printResultInFile(Matrix* const matrix);



void startLabInProcessMode()
{
    pid_t pid;

    auto matrix = new Matrix(ROW, COL);
    auto shMem = createSharedMemory(matrix->size * sizeof(matrix->firstMatrix[0]));

    pid = fork();

    switch (pid)
    {
        case -1:
            exit(-1);
        case  0:
            generateMatrix(shMem, matrix->size);
            exit(0);
        default: 
            wait(&pid);
            break;
    }

    for (unsigned i = 0; i < matrix->size; ++i)
        matrix->firstMatrix[i] = shMem[i];

    pid = fork();

    switch (pid)
    {
        case -1: 
            exit(-1);
        case  0:
            generateMatrix(shMem, matrix->size);
            exit(0);
        default: 
            wait(&pid);
            break;
    }

    for (unsigned i = 0; i < matrix->size; ++i)
        matrix->secondMatrix[i] = shMem[i];

    pid = fork();

        switch (pid)
    {
        case -1: 
            exit(-1);
        case  0:
            sumMatrix(shMem, matrix->size, matrix->firstMatrix.data(), matrix->secondMatrix.data());
            exit(0);
        default: 
            wait(&pid);
            break;
    }

    for (unsigned i = 0; i < matrix->size; ++i)
        matrix->result[i] = shMem[i];

    pid = fork();

    switch (pid)
    {
        case -1: 
            exit(-1);
        case  0:
            printResultInFile(matrix);
            exit(0);
        default: 
            wait(&pid);
            break;
    }
    
    delete matrix;
}


int* createSharedMemory(const unsigned size)
{
    int protection = PROT_WRITE | PROT_READ;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    return static_cast<int*>(mmap(nullptr, size, protection, visibility, -1, 0));
}


void generateMatrix(int* const shMem, const unsigned size)
{
    int arr[size];

    std::mt19937 engine;
    engine.seed(getpid());
    std::uniform_int_distribution<int> range(1, 49);

    for (unsigned i = 0; i < size; ++i)
        arr[i] = range(engine);

    memcpy(shMem, arr, sizeof(arr));
}


void sumMatrix(int* const shMem, const unsigned size, int* const firstMatrix, int* const secondMatrix)
{
    for (unsigned i = 0; i < size; ++i)
        shMem[i] = firstMatrix[i] + secondMatrix[i];
}

void printResultInFile(Matrix* const matrix)
{
    std::ofstream resultFile("./result.txt");

    if (resultFile)
    {
        resultFile << *matrix;
        resultFile.close();
    }
}
