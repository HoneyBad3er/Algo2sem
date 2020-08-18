//Требуется найти в связном графе остовное дерево минимального веса.
//Воспользуйтесь алгоритмом Крускала.
//
//Формат ввода
//Первая строка входного файла содержит два натуральных числа n и m — количество вершин и ребер графа соответственно.
//Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается тремя натуральными числами
//bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100 000). n ≤ 20 000, m ≤ 100 000.
//Граф является связным.
//
//Формат вывода
//Первая строка выходного файла должна содержать одно натуральное число — вес минимального остовного дерева.


#include <iostream>
#include <vector>
#include <algorithm>

//_________DSU_________
class DSU {
public:
    explicit DSU(size_t vCount);
    size_t get_root(size_t v);
    void union_trees(size_t v1, size_t v2);
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;
};

DSU::DSU(size_t vCount): parent(vCount, 0), rank(vCount, 0) {
    for (size_t i = 0; i < vCount; ++i) {
        parent[i] = i;
    }
}

size_t DSU::get_root(size_t v) {
    if (v == parent[v])
        return v;
    return parent[v] = get_root(parent[v]);
}
 
void DSU::union_trees(size_t v1, size_t v2) {
    v1 = get_root(v1);
    v2 = get_root(v2);
    if (v1 != v2) {
        if (rank[v1] < rank[v2]) {
            std::swap(v1, v2);
        }
        parent[v2] = v1;
        if (rank[v1] == rank[v2]) {
            ++rank[v1];
        }
    }
}

//_________EDGE_________
struct Edge {
    size_t begin;
    size_t end;
    size_t cost;
};

bool operator < (const Edge& lhs, const Edge& rhs) {
    return lhs.cost < rhs.cost;
}

//_________GRAPH_________
class Graph {
public:
    explicit Graph(size_t vCount, size_t eCount);
    void AddEdge(size_t begin, size_t end, size_t cost);
    const std::vector<Edge>& GetEdges() const;
    const size_t VertexCount() const;
    const size_t EdgesCount() const;
private:
    const size_t edgesCount;
    const size_t vertexCount;
    std::vector<Edge> data;
};

Graph::Graph(size_t vCount, size_t eCount): vertexCount(vCount),
                                            edgesCount(eCount) {
    data.reserve(eCount);
}

void Graph::AddEdge(size_t begin, size_t end, size_t cost) {
    data.push_back({begin, end, cost});
}

const std::vector<Edge>& Graph::GetEdges() const {
    return data;
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

const size_t Graph::EdgesCount() const {
    return edgesCount;
}

size_t GetMstWeight(const Graph& graph) {
    size_t mstWeight = 0;
    DSU dsu(graph.VertexCount());
    std::vector<Edge> edges = graph.GetEdges();
    std::sort(edges.begin(), edges.end());
    for (const Edge& edge : edges) {
        if (dsu.get_root(edge.begin) != dsu.get_root(edge.end)) {
            mstWeight += edge.cost;
            dsu.union_trees(edge.begin, edge.end);
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
    Graph graph(vertexCount, edgesCount);
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> begin >> end >> cost;
        graph.AddEdge(begin - 1, end - 1, cost);
    }
    std::cout << GetMstWeight(graph) << std::endl;
    return 0;
}
