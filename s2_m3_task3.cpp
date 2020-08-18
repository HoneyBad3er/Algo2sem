// Вам дан неориентированный граф, состоящий из n вершин. На каждой вершине записано число;
// число, записанное на вершине i, равно ai. Изначально в графе нет ни одного ребра.
// Вы можете добавлять ребра в граф за определенную стоимость. За добавление ребра между
//  вершинами x и y надо заплатить ax + ay монет. Также существует m специальных предложений, 
//  каждое из которых характеризуется тремя числами x, y и w, и означает, что можно добавить 
//  ребро между вершинами x и y за w монет. Эти специальные предложения не обязательно использовать: 
//  если существует такая пара вершин x и y, такая, что для нее существует специальное предложение, 
//  можно все равно добавить ребро между ними за ax + ay монет.
// Сколько монет минимально вам потребуется, чтобы сделать граф связным? Граф является связным, 
// если от каждой вершины можно добраться до любой другой вершины, используя только ребра этого графа.

// Формат ввода
// В первой строке заданы два целых числа n и m (1 ≤ n ≤ 2 ⋅ 105, 0 ≤ m ≤ 2 ⋅ 105) — количество 
// вершин в графе и специальных предложений, соответственно.
// Во второй строке заданы n целых чисел  (1 ≤ ai ≤ 1012) — числа, записанные на вершинах.
// Затем следуют m строк, в каждой из которых заданы три целых числа x, y и w (1 ≤ x, y ≤ n, 1 ≤ w ≤ 1012, x ≠ y), 
// обозначающие спецпредложение: можно добавить ребро между вершинами x и y за w монет.

// Формат вывода
// Выведите одно целое число — минимальное количество монет, которое необходимо потратить, чтобы сделать граф связным.

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
    explicit Edge(size_t begin, size_t end, size_t cost);
    size_t begin;
    size_t end;
    size_t cost;
};

Edge::Edge(size_t begin, size_t end, size_t cost) : begin(begin), end(end), cost(cost) {}

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
    data.reserve(vCount + eCount);
}

void Graph::AddEdge(size_t begin, size_t end, size_t cost) {
    data.push_back(Edge(begin, end, cost));
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
    sort(edges.begin(), edges.end());
    for (const Edge& edge : edges) {
        if (dsu.get_root(edge.begin) != dsu.get_root(edge.end)) {
            mstWeight += edge.cost;
            dsu.union_trees(edge.begin, edge.end);
        }
    }
    return mstWeight;
}

int main() {
    size_t vertexCount, edgesCount = 0;
    size_t begin, end = 0;
    long long cost = 0;
    std::cin >> vertexCount >> edgesCount;
    Graph graph(vertexCount, edgesCount);
    std::vector<size_t> a(vertexCount);
    long long minA = std::numeric_limits<long long>::max();
    size_t minV = 0;
    for (size_t i = 0; i < vertexCount; ++i) {
        std::cin >> a[i];
        if (a[i] < minA) {
            minV = i;
            minA = a[i];
        }
    }
    for (size_t i = 0; i < vertexCount; ++i) {
        graph.AddEdge(i, minV, minA + a[i]);
    }
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> begin >> end >> cost;
        graph.AddEdge(begin - 1, end - 1, cost);
    }
    std::cout << GetMstWeight(graph) << std::endl;
    return 0;
}
