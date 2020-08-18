//Шреку необходимо пробраться в башню, чтобы спасти прекрасную принцессу Фиону. И сейчас его путь лежит через старый
//полуразвалившийся мост. Осёл очень боится упасть с этого моста в лавовую реку под ним и отказывается идти дальше, пока мост
//не будет полностью починен. Шрек не может идти без друга и решил отремонтировать его.
//Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми. У Шрека есть только дощечки
//размера 1 × 2, установка которых занимает у него a секунд, и 1 × 1, установка которых занимает b секунд. Ему необходимо
//закрыть все пустые клетки, причем только их, не накладывая дощечки друг на друга.
//Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.
//
//Формат ввода
//Первая строка входного файла содержит 4 целых числа n, m, a, b (1 ≤ n, m ≤ 100, |a| ≤ 1 000, |b| ≤ 1 000). Каждая из
//последующих n строк содержит по m символов: символ  «.» (точка) обозначает целую клетку моста, а символ «*» (звёздочка) —
//пустую.
//
//Формат вывода
//В выходной файл выведите одно число — минимальное количество секунд, через которое наши герои смогут продолжить свой путь,
//закрыв пустые клетки моста (их и только их).


#include <iostream>
#include <vector>
#include <utility>
#include <set>


class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV);
    const std::vector<size_t>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector<std::vector<size_t>> data;
};

Graph::Graph(size_t vCount) : vertexCount(vCount), data(vCount) {}

void Graph::AddEdge(size_t firstV, size_t secondV) {
    data[firstV].push_back(secondV);
    data[secondV].push_back(firstV);
}

const std::vector<size_t>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

bool FindMatching(const Graph& graph, size_t vertex,
              std::vector<bool>& visited,
              std::vector<int>& matching) {
    if (visited[vertex]) {
        return false;
    }
    visited[vertex] = true;
    for (size_t u : graph.GetNeighbours(vertex)) {
        if (matching[u] == -1 || FindMatching(graph, matching[u], visited, matching)) {
            matching[u] = static_cast<int>(vertex);
            return true;
        }
    }
    return false;
}

int GetMinTime(const Graph& graph, int singleCost, int pairCost, size_t unbroken) {
    if (pairCost >= 2 * singleCost) {
        return (static_cast<int>(graph.VertexCount())
                - static_cast<int>(unbroken)) * singleCost;
    }
    std::vector<bool> visited(graph.VertexCount(), false);
    std::vector<int> matching(graph.VertexCount(), -1);
    for (size_t i = 0; i < graph.VertexCount(); ++i) {
        visited.assign(graph.VertexCount(), false);
        FindMatching(graph, i, visited, matching);
    }
    int pairCount = 0;
    for (size_t v = 0; v < graph.VertexCount(); ++v) {
        if (matching[v] != -1) {
            ++pairCount;
        }
    }
    pairCount /= 2;
    int singleCount = static_cast<int>(graph.VertexCount())
                      - static_cast<int>(unbroken) - 2 * pairCount;
    
    return singleCount * singleCost + pairCount * pairCost;
}

size_t MatchRow(Graph& graph, const std::string& row, size_t rowNum) {
    size_t result = 0;
    size_t start = rowNum * row.size();
    for (size_t i = 0; (i + 1) < row.size(); ++i) {
        if (row[i] == '*' && row[i + 1] == '*') {
            graph.AddEdge(start + i, start + i + 1);
        }
        if (row[i] == '.') {
            ++result;
        }
    }
    return *row.rbegin() == '.' ? ++result : result;
}

void MatchCol(Graph& graph, const std::string& first,
              const std::string& second, size_t rowNum) {
    size_t start = rowNum * first.size();
    for (size_t i = 0; i < first.size(); ++i) {
        if (first[i] == '*' && second[i] == '*') {
            graph.AddEdge(start + i, start + first.size() + i);
        }
    }
}

int main() {
    size_t n, m = 0;
    int singleCost, pairCost = 0;
    std::cin >> n >> m >> pairCost >> singleCost;
    Graph graph(n * m);
    size_t unbroken = 0;
    std::string prev;
    std::string current;
    std::cin >> prev;
    unbroken += MatchRow(graph , prev, 0);
    for (size_t rowNum = 1; rowNum < n; ++rowNum) {
        std::cin >> current;
        unbroken += MatchRow(graph, current, rowNum);
        MatchCol(graph, current, prev, rowNum - 1);
        prev = current;
    }
    std::cout << GetMinTime(graph, singleCost, pairCost, unbroken) << std::endl;
    return 0;
}
