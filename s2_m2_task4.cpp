// Рик отправляет Морти в путешествие по N вселенным. У него есть список всех существующих однонаправленных телепортов. 
// Чтобы Морти не потерялся, Рику необходимо узнать, между какими вселенными существуют пути, а между какими нет. Помогите ему в этом!

// Формат ввода
// На первой строке задано количество вселенных N (1 ≤ N ≤ 1 000). Следующие N строк имеют длину N, 
// состоят из нулей и единиц и задают матрицу смежности вселенных. Единица в i-й строке j-м столбце обозначает телепорт из i-й вселенной в j-ю.

// Формат вывода
// Выведите матрицу, где единица в i-й строке j-м столбце обозначает существование пути из i-й вселенной в j-ю и ноль — его отсутствие.

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <bitset>


class Graph {
public:
    explicit Graph(size_t vCount): vertexCount(vCount) {
        data.resize(vertexCount);
    }
    void AddEdge(size_t firstV, size_t secondV);
    void GetTrCL();
    void PrintData() const;
private:
    const size_t vertexCount;
    std::vector< std::bitset<1024> > data;
};

void Graph::AddEdge(size_t firstV, size_t secondV) {
    data[firstV][secondV] = 1;
}

void Graph::GetTrCL() {
    for (size_t cur_row = 0; cur_row < vertexCount; ++cur_row) {
        for (size_t cur_col = 0; cur_col < vertexCount; ++cur_col) {
            if (data[cur_col][cur_row]) {
                data[cur_col] |= data[cur_row];
            }
        }
    }
}

void Graph::PrintData() const {
    for (size_t cur_row = 0; cur_row < vertexCount; ++cur_row) {
        for (size_t cur_col = 0; cur_col < vertexCount; ++cur_col) {
            std::cout << data[cur_row][cur_col];
        }
        std::cout << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t universeQty = 0;
    std::cin >> universeQty;
    Graph graph(universeQty);
    std::string s;
    for (size_t i = 0; i < universeQty; ++i) {
        std::cin >> s;
        for (size_t j = 0; j < universeQty; ++j) {
            if (s[j] == '1') {
                graph.AddEdge(i, j);
            }
        }
    }
    graph.GetTrCL();
    graph.PrintData();
    return 0;
}
