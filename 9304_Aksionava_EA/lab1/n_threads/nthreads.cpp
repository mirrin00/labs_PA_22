#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

void thread_func(const int i) {
    cout << "hello from thread: " << i << endl;
}

ifstream open_file_read(string file_name) {
  ifstream file;
  file.open(file_name, ios::in);
  return file;
}

void read_matrix(int* matrix, int rows, int columns, string file_name) {
  ifstream file = open_file_read(file_name);
  string delimiter = " ";
  for(int i = 0; i < rows; i++){
    string line;
    getline(file, line);
    for(int j = 0; j < columns; j++){
      matrix[i * columns + j] = stoi(line.substr(0, line.find(delimiter)));
      line = line.substr(line.find(delimiter) + 1);
    }
  }
  file.close();
}

void read_first_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_1.txt";
  read_matrix(matrix, rows, columns, matrix_name);
}

void read_second_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_2.txt";
  read_matrix(matrix, rows, columns, matrix_name);
}

void init_block(int* first_matrix, int* second_matrix, int rows, int columns) {
  read_first_matrix(first_matrix, rows, columns);
  read_second_matrix(second_matrix, rows, columns);
}

void sum_matrix_elem(int* result, int first_matrix_elem, int second_matrix_elem, int index) {
  result[index] = first_matrix_elem + second_matrix_elem;
}

void sum_matrix(int* result, int* first_matrix, int* second_matrix, int size, int thread_count){
  std::thread myThreads[thread_count];
  int cur_thread_count = thread_count;
  for (int elem_num = 0; elem_num < size; elem_num += cur_thread_count){
    for (int thread_num = 0; thread_num < cur_thread_count; thread_num++) {
      int index = elem_num + thread_num;
      myThreads[thread_num] = std::thread{sum_matrix_elem, result, first_matrix[index], second_matrix[index], index};
      if (elem_num + cur_thread_count >= size) {
          cur_thread_count = size - elem_num;
      }
    }
    for (int thread_num = 0; thread_num < thread_count; thread_num++) {
      myThreads[thread_num].join();
    }
  }
}

//void sum_matrix_row(int* result, int* first_matrix, int* second_matrix, int row_index, int columns) {
//  for (int i = 0; i < columns; i++) {
//    int cur_index = row_index + i * columns;
//    result[cur_index] = first_matrix[cur_index] + second_matrix[cur_index];
//  }
//}
//
//void sum_matrix(int* result, int* first_matrix, int* second_matrix, int rows, int columns, int thread_count){
//  std::thread myThreads[thread_count];
//  int size = rows * columns;
//  int cur_thread_count = thread_count;
//  for (int row_num = 0; row_num < rows; row_num += cur_thread_count){
//    for (int thread_num = 0; thread_num < cur_thread_count; thread_num++) {
//      int row_index = row_num + thread_num;
//      myThreads[thread_num] = std::thread{sum_matrix_row, result, first_matrix, second_matrix, row_index, columns};
//      if (row_num + cur_thread_count >= size) {
//          cur_thread_count = rows - row_num;
//      }
//    }
//    for (int thread_num = 0; thread_num < thread_count; thread_num++) {
//      myThreads[thread_num].join();
//    }
//  }
//}

ofstream open_file_write(string file_name) {
  ofstream file;
  file.open(file_name, ios::out);
  return file;
}

void write_result(int* result, int rows, int columns) {
  ofstream file = open_file_write("result.txt");
  int cur_symbol_num = 0;
  while (cur_symbol_num < rows * columns) {
    file << result[cur_symbol_num] << " ";
    cur_symbol_num++;
    if (cur_symbol_num % columns == 0) {
      file << endl;
    }
  }
  file.close();
}

int main() {

  int rows = 10000;
  int columns = 5000;
  int thread_count = 10000;
  int* first_matrix = new int[rows * columns];
  int* second_matrix = new int[rows * columns];
  int* result = new int[rows * columns];
  thread read_t;
  thread sum_t;
  thread write_t;

  cout << "read_t exists" << endl;

  read_t = thread{init_block, first_matrix, second_matrix, rows, columns};
  read_t.join();

  cout << "read" << endl;

  clock_t start_time = clock();

  sum_t = thread{ sum_matrix, result, first_matrix, second_matrix, rows * columns, thread_count};
//  sum_t = thread{ sum_matrix, result, first_matrix, second_matrix, rows, columns, thread_count};
  sum_t.join();

  printf("Time taken: %.6f\n", (double)(clock() - start_time)/CLOCKS_PER_SEC);

  cout << "sum" << endl;

  write_t = thread{ write_result, result, rows, columns };
  write_t.join();

  cout << "write" << endl;

  delete[] first_matrix;
  delete[] second_matrix;
  delete[] result;
  return 0;
}