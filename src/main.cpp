#include <random>
#include <iostream>
#include "WorkerInterface.hpp"



void PrintMatrix(Matrix& m) {
    for (size_t i = 0; i < m.height; ++i) {
        for (size_t j = 0; j < m.width; ++j) {
            std::cout << m.data[i * m.width + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    auto worker = get_new_worker();
    Matrix mtx {
        .data = {
            1, 2, 3,    
            4, 5, 6
        },
    .width = 3,
    .height = 2
    };
    auto t  = TransposeMatrix(mtx);
    PrintMatrix(mtx);
    PrintMatrix(t);

}