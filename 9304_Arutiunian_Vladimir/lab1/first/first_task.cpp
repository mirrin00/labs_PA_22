#include "first_task.h"

int main() {
    process(inputMatrices);
    process(sumMatrices);
    process(printMatrices);
    return 0;
}

void process(const std::function<void()>& childFunction) {
    auto pid = fork();

    if (fork_pid::kChild == pid) {
        childFunction();
        exit(0);
    } else if (fork_pid::kError != pid) {
        wait(&pid);
    }
}

void inputMatrices() {
    std::ifstream matrix_input_file(filenames::kMatrixInputFilename);

    auto first_matrix = Matrix();
    auto second_matrix = Matrix();

    matrix_input_file >> first_matrix;
    printPidWithText("First matrix has been successfully read. Pid: ");

    matrix_input_file >> second_matrix;
    printPidWithText("Second matrix has been successfully read. Pid: ");

    matrix_input_file.close();

    const int shared_memory_id = createOrGetSharedMemory();
    const auto shared_memory_pointer = (int*) attachToSharedMemory(shared_memory_id);

    auto after_first_matrix_pointer = first_matrix.writeByPointer(shared_memory_pointer);
    second_matrix.writeByPointer(after_first_matrix_pointer);
}

void printPidWithText(const std::string& text) {
    std::cout << text << getpid() << '\n';
}

int createOrGetSharedMemory() {
    const int kKey = 1234;
    const int memorySize = 1024;

    auto id = shmget(kKey, memorySize, 0644 | IPC_CREAT);

    if (id == -1) {
        throw std::runtime_error("Error during creating/getting shared memory");
    }

    std::cout << "Shared memory successfully created/got with id: " << id << '\n';

    return id;
}

void* attachToSharedMemory(int shared_memory_id) {
    auto pointer = shmat(shared_memory_id, nullptr, 0);

    if (pointer == (void*) -1) {
        throw std::runtime_error("Error during attaching to shared memory");
    }

    std::cout << "Process successfully attached to shared memory. Pointer address: " << pointer << '\n';

    return pointer;
}

void sumMatrices() {
    const int shared_memory_id = createOrGetSharedMemory();
    auto shared_memory_pointer = (int*) attachToSharedMemory(shared_memory_id);

    auto first_matrix_with_next_pointer = Matrix::readByPointer(shared_memory_pointer);
    auto const& first_matrix = first_matrix_with_next_pointer.first;

    auto second_matrix_with_next_pointer = Matrix::readByPointer(first_matrix_with_next_pointer.second);
    auto const& second_matrix = second_matrix_with_next_pointer.first;

    auto summed_matrix = first_matrix + second_matrix;
    summed_matrix.writeByPointer(shared_memory_pointer);
    printPidWithText("Sum of matrices has been successfully calculated. Pid: ");
}

void printMatrices() {
    const int shared_memory_id = createOrGetSharedMemory();
    auto shared_memory_pointer = (int*) attachToSharedMemory(shared_memory_id);

    auto matrix_with_next_pointer = Matrix::readByPointer(shared_memory_pointer);
    auto const& matrix = matrix_with_next_pointer.first;

    std::ofstream matrix_output_file(filenames::kMatrixOutputFilename);
    matrix_output_file << matrix;

    printPidWithText("Matrix has been successfully printed. Pid: ");
    matrix_output_file.close();
}