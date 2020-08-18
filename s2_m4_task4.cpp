// Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
// Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
// Запросы генерируются следующим образом. Заданы числа a1, a2 и числа x, y и z.
// Числа a3, ..., a2m генерируются следующим образом: ai = (x ⋅ ai-2 + y ⋅ ai-1 + z) mod n.
// Первый запрос имеет вид (a1, a2). Если ответ на i-1-й запрос равен v, то i-й запрос имеет вид ((a2i-1 + v) mod n, a2i).
// Для решения задачи можно использовать метод двоичного подъёма.

// Формат ввода
// Первая строка содержит два числа: n и m.
// Корень дерева имеет номер 0.
// Вторая строка содержит n-1 целых чисел, i-е из этих чисел равно номеру родителя вершины i.
// Третья строка содержит два целых числа в диапазоне от 0 до n-1: a1 и a2.
// Четвертая строка содержит три целых числа: x, y и z, эти числа неотрицательны и не превосходят 109.

// Формат вывода
// Выведите в выходной файл сумму номеров вершин — ответов на все запросы.

#include <iostream>
#include <vector>
#include <cmath>

class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV);
    const std::vector<size_t>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;

private:
    size_t vertexCount;
    std::vector<std::vector<size_t>> data;
};

Graph::Graph(size_t vCount): vertexCount(vCount), data(vCount) {}

void Graph::AddEdge(size_t firstV, size_t secondV) {
    data[firstV].push_back(secondV);
    data[secondV].push_back(firstV);
}

const std::vector<size_t>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

const size_t Graph::VertexCount() const {
    return data.size();
}

class LcaFinder {
public:
    explicit LcaFinder(const Graph& graph);
    size_t getLCA(size_t v1, size_t v2) const;
    
private:
    void buildAncestorTable(size_t currentV, size_t parent);
    bool isAncestor(size_t v1, size_t v2) const;
    
    size_t currentTime;
    const Graph& graph;
    std::vector<bool> used;
    std::vector<std::pair<size_t, size_t>> time;
    std::vector<std::vector<size_t>> ancestorTable;
};

LcaFinder::LcaFinder(const Graph& graph) : currentTime(0), graph(graph), used(graph.VertexCount(), false),
                                           time(graph.VertexCount()), ancestorTable(graph.VertexCount()) {
    
    size_t pow = static_cast<size_t>(std::log2(graph.VertexCount()));
    for (size_t i = 0; i < ancestorTable.size(); ++i) {
       ancestorTable[i].resize(pow + 1);
    }
    buildAncestorTable(0, 0);
}

void LcaFinder::buildAncestorTable(size_t currentV, size_t parent) {
    time[currentV].first = currentTime++;
    used[currentV] = true;
    ancestorTable[currentV][0]  = parent;
    for (size_t i = 1; i < ancestorTable[0].size(); ++i) {
        ancestorTable[currentV][i] = ancestorTable[ancestorTable[currentV][i - 1]][i - 1];
    }
    for (size_t vertex : graph.GetNeighbours(currentV)) {
        if (!used[vertex]) {
            buildAncestorTable(vertex, currentV);
        }
    }
    time[currentV].second = currentTime;
}

bool LcaFinder::isAncestor(size_t v1, size_t v2) const {
    return time[v1].first <= time[v2].first && time[v2].second <= time[v1].second;
}

size_t LcaFinder::getLCA(size_t v1, size_t v2) const {
    if (v1 == v2 || isAncestor(v1, v2)) {
        return v1;
    }
    if (isAncestor(v2, v1)) {
        return v2;
    }
    for (int i = static_cast<int>(ancestorTable[0].size()) - 1; i >= 0; --i) {
        if (!isAncestor(ancestorTable[v1][i], v2)) {
            v1 = ancestorTable[v1][i];
        }
    }
    return ancestorTable[v1][0];
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    size_t vertex;
    Graph graph(n);
    for (size_t i = 1; i < n; ++i) {
        std::cin >> vertex;
        graph.AddEdge(i, vertex);
    }
    size_t a1, a2, x, y, z;
    std::cin >> a1 >> a2;
    std::cin >> x >> y >> z;
    LcaFinder lca(graph);
    size_t currentLca = lca.getLCA(a1, a2);
    size_t lcaSum = currentLca;
    for (size_t i = 1; i < m; ++i) {
            a1 = (x * a1 % n + y * a2 % n + z % n) % n;
            a2 = (x * a2 % n + y * a1 % n + z % n) % n;
            currentLca = lca.getLCA((a1 + currentLca) % n, a2);
            lcaSum += currentLca;
    }
    std::cout << lcaSum <<'\n';
    return 0;
}
