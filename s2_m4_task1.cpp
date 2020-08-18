// Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.
// Для решения задачи используйте структуру данных Sparse Table. Требуемое время обработки каждого диапазона O(1). 
// Время подготовки структуры данных .

// Формат ввода
// В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
// Следующие N целых чисел задают последовательность. Далее вводятся M пар чисел - границ диапазонов.
// Гарантируется, что каждый диапазон содержит как минимум 2 элемента.

// Формат вывода
// Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику. По одному числу в строке.

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

struct MinPair {
    explicit MinPair(int min1, int min2);
    int min1;
    int min2;
};

MinPair::MinPair(int min1, int min2) : min1(min1), min2(min2) {}

class SparseTable {
public:
    explicit SparseTable(const std::vector<int>& mas);
    int getSecondMin(size_t l, size_t r);
private:
    std::vector<std::vector<MinPair>> table;
    MinPair getMinPair(const MinPair& l, const MinPair& r);
};

SparseTable::SparseTable(const std::vector<int>& data) : table(int(std::log2(data.size()) + 1), 
                                                               std::vector<MinPair>(data.size(), MinPair(0,0))) {
            
    for (size_t i = 0; i < data.size(); ++i) {
        table[0][i] = MinPair(data[i], std::numeric_limits<int>::max());
    }
    for (size_t k = 1; k < table.size(); ++k) {
        for (size_t i = 0; i <= data.size() - (1 << k); ++i) {
            table[k][i] = getMinPair(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
        }
    }
}

MinPair SparseTable::getMinPair(const MinPair& l, const MinPair& r) {
    if (l.min1 < r.min1) {
        return MinPair(l.min1, std::min(l.min2, r.min1));
    }
    return MinPair(r.min1, std::min(r.min2, l.min1));
}

int SparseTable::getSecondMin(size_t l, size_t r) {
    int k = int(std::log2(r - l + 1));
    if (table[k][l].min1 == table[k][r - (1 << k) + 1].min1) {
        return std::min(table[k][l].min2, table[k][r - (1 << k) + 1].min2);
    } else {
        return getMinPair(table[k][l], table[k][r - (1 << k) + 1]).min2;
    }
}

int main() {
    size_t N = 0;
    size_t M = 0;
    std::cin >> N >> M;
    std::vector<int> mas(N);
    for (size_t i = 0; i < N; ++i) {
        std::cin >> mas[i];
    }
    SparseTable table(mas);
    size_t l = 0;
    size_t r = 0;
    for (size_t i = 0; i < M; ++i) {
        std::cin >> l >> r;
        std::cout << table.getSecondMin(l - 1, r -  1) << "\n";
    }
    return 0;
}
