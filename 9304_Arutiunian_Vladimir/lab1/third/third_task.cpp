#include "third_task.h"

int main() {
    int min_threads_count = 1;
    int max_threads_count = 100;
    for (int i = min_threads_count; i <= max_threads_count; i += (i < 10 ? 1 : 10)) {
        long long elapsedTime = startFlow(i);
        std::cout << "Threads count: " << i << ". Elapsed time: " << elapsedTime << "\n\n";
    }
    return 0;
}

long long startFlow(int threads_count) {
    auto first_matrix = Matrix();
    auto second_matrix = Matrix();

    std::thread inputThread(inputMatrices, std::ref(first_matrix), std::ref(second_matrix));
    inputThread.join();
    

    auto begin = std::chrono::steady_clock::now();

    std::promise<Matrix> summed_matrix_promise;
    auto summed_matrix_future = summed_matrix_promise.get_future();

    std::thread sumThread(sumMatrices, std::ref(first_matrix), std::ref(second_matrix), std::move(summed_matrix_promise), threads_count);
    sumThread.join();

    auto summed_matrix = summed_matrix_future.get();

    auto end = std::chrono::steady_clock::now();


    std::thread printThread(printMatrices, std::ref(summed_matrix));
    printThread.join();

    return getElapsedTime(begin, end);
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

void sumMatrices(const Matrix& first_matrix, const Matrix& second_matrix, std::promise<Matrix> summed_matrix_promise, int threads_count) {
    auto summed_matrix = first_matrix.parallelSum(second_matrix, threads_count);
    printThreadIdWithText("Sum of matrices has been successfully calculated");

    summed_matrix_promise.set_value(summed_matrix);
}


void printMatrices(const Matrix& matrix) {
    std::ofstream matrix_output_file(filenames::kMatrixOutputFilename);
    matrix_output_file << matrix;
    printThreadIdWithText("Matrix has been successfully printed");
    matrix_output_file.close();
}

long long getElapsedTime(std::chrono::time_point<std::chrono::steady_clock> begin, std::chrono::time_point<std::chrono::steady_clock> end) {
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}
