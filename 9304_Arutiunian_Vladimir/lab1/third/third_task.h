#ifndef LABS_PA_22_THIRD_TASK_H
#define LABS_PA_22_THIRD_TASK_H

#include <iostream>
#include <thread>
#include <fstream>
#include <future>
#include <chrono>

#include "constant/filenames.h"
#include "matrix/matrix.h"

long long startFlow(int threads_count);

void inputMatrices(Matrix& first_matrix, Matrix& second_matrix);

void printThreadIdWithText(const std::string& text);

void sumMatrices(const Matrix& first_matrix, const Matrix& second_matrix, std::promise<Matrix> summed_matrix_promise, int threads_count);

void printMatrices(const Matrix& matrix);

long long getElapsedTime(std::chrono::time_point<std::chrono::steady_clock> begin, std::chrono::time_point<std::chrono::steady_clock> end);

#endif //LABS_PA_22_THIRD_TASK_H
