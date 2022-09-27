#ifndef LABS_PA_22_SECOND_TASK_H
#define LABS_PA_22_SECOND_TASK_H

#include <iostream>
#include <thread>
#include <fstream>
#include <future>

#include "constant/filenames.h"
#include "matrix/matrix.h"

void inputMatrices(Matrix& first_matrix, Matrix& second_matrix);

void printThreadIdWithText(const std::string& text);

void sumMatrices(const Matrix& first_matrix, const Matrix& second_matrix, std::promise<Matrix> summed_matrix_promise);

void printMatrices(const Matrix& matrix);

#endif //LABS_PA_22_SECOND_TASK_H
