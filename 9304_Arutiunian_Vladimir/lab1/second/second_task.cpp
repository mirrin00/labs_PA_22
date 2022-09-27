#include "second_task.h"

int main() {
    auto first_matrix = Matrix();
    auto second_matrix = Matrix();

    std::thread inputThread(inputMatrices, std::ref(first_matrix), std::ref(second_matrix));
    inputThread.join();


    std::promise<Matrix> summed_matrix_promise;
    auto summed_matrix_future = summed_matrix_promise.get_future();

    std::thread sumThread(sumMatrices, std::ref(first_matrix), std::ref(second_matrix), std::move(summed_matrix_promise));
    sumThread.join();

    auto summed_matrix = summed_matrix_future.get();


    std::thread printThread(printMatrices, std::ref(summed_matrix));
    printThread.join();
    return 0;
}

void inputMatrices(Matrix& first_matrix, Matrix& second_matrix) {
    std::ifstream matrix_input_file(filenames::kMatrixInputFilename);

    matrix_input_file >> first_matrix;
    printThreadIdWithText("First matrix has been successfully read");

    matrix_input_file >> second_matrix;
    printThreadIdWithText("Second matrix has been successfully read");

    matrix_input_file.close();
}

void printThreadIdWithText(const std::string& text) {
    std::cout << text << ". ThreadId: " << std::this_thread::get_id() << '\n';
}

void sumMatrices(const Matrix& first_matrix, const Matrix& second_matrix, std::promise<Matrix> summed_matrix_promise) {
    auto summed_matrix = first_matrix + second_matrix;
    printThreadIdWithText("Sum of matrices has been successfully calculated");

    summed_matrix_promise.set_value(summed_matrix);
}


void printMatrices(const Matrix& matrix) {
    std::ofstream matrix_output_file(filenames::kMatrixOutputFilename);
    matrix_output_file << matrix;
    printThreadIdWithText("Matrix has been successfully printed");
    matrix_output_file.close();
}
