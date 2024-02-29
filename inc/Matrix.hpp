#pragma once
#include <vector>

struct Matrix {
    std::vector<int> data;
    unsigned width, height;
};



template<class RandomIterator>
void transpose(RandomIterator first, RandomIterator last, int m)
{
    const int m1 = (last - first - 1);
    const int n   = (last - first) / m;
    std::vector<bool> visited(last - first);
    RandomIterator cycle = first;
    while (++cycle != last) {
        if (visited[cycle - first])
            continue;
        int a = cycle - first;
        do  {
            a = a == m1 ? m1 : (n * a) % m1;
            std::swap(*(first + a), *cycle);
            visited[a] = true;
        } while ((first + a) != cycle);
    }
}


/**
  * Функция транспонирования матрицы.
  * 
  * @param matrix - входная матрица для транспонирования.
  * @return - возвращается транспонированная матрица
  *
  */
Matrix TransposeMatrix(Matrix matrix) {
    if (matrix.data.empty()) return matrix;
    transpose(matrix.data.begin(),matrix.data.end(),matrix.width);
    std::swap(matrix.height,matrix.width);
    return matrix;
}

