#include <iostream>
#include <fstream>
#include <vector>

void printMatrix(std::ofstream& output, int height, int width);

int main() {
    int height = 15;
    int width = 15;

    std::ofstream output("matrices.txt");
    std::vector<std::vector<int>> matrix;

    printMatrix(output, height, width);
    printMatrix(output, height, width);

    output.close();
    return 0;
}

void printMatrix(std::ofstream& output, int height, int width) {
    output << height << ' ' << width << '\n';
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            output << i * j;
            if (j < height - 1) {
                output << ' ';
            }
        }
        output << '\n';
    }
}