//Требуется найти в связном графе остовное дерево минимального веса.
//Воспользуйтесь алгоритмом Прима.
//
//Формат ввода
//Первая строка входного файла содержит два натуральных числа n и m — количество вершин и ребер графа соответственно.
//Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается тремя натуральными числами
//bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100 000). n ≤ 5 000, m ≤ 100 000.
//Граф является связным.
//
//Формат вывода
//Первая строка выходного файла должна содержать одно натуральное число — вес минимального остовного дерева.


#include <vector>
#include <set>
#include <iostream>
#include <utility>
#include <limits>


struct Edge {
    size_t vertex;
    size_t cost;
};

class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV, size_t cost);
    const std::vector<Edge>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector<std::vector<Edge>> data;
};

Graph::Graph(size_t vCount): vertexCount(vCount), data(vCount) {}

void Graph::AddEdge(size_t firstV, size_t secondV, size_t cost) {
    data[firstV].push_back({secondV, cost});
    data[secondV].push_back({firstV, cost});
}

const std::vector<Edge>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

size_t GetMstWeight(const Graph& graph) {
    size_t mstWeight = 0;
    std::vector<size_t> minDist(graph.VertexCount(), std::numeric_limits<size_t>::max());
    std::vector<bool> inMst(graph.VertexCount(), false);
    minDist[0] = 0;
    std::set< std::pair<size_t, size_t> > rest; //{вес, вершина}
    rest.insert({0, 0});
    while (!rest.empty()) {
        auto [cost, v] = *rest.begin();
        rest.erase(rest.begin());
        inMst[v] = true;
        mstWeight += cost;
        for (auto [curV, curCost] : graph.GetNeighbours(v)) {
            if (curCost < minDist[curV] && !inMst[curV]) {
                rest.erase({minDist[curV], curV});
                minDist[curV] = curCost;
                rest.insert({curCost, curV});
            }
        }
    }
    return mstWeight;
}

int main() {
    size_t vertexCount = 0;
    size_t edgesCount = 0;
    size_t begin = 0;
    size_t end = 0;
    size_t cost = 0;
    std::cin >> vertexCount >> edgesCount;
    Graph graph(vertexCount);
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> begin >> end >> cost;
        graph.AddEdge(begin - 1, end - 1, cost);
    }
    std::cout << GetMstWeight(graph) << std::endl;
    return 0;
}
