// A. За молоком
// Леон и Матильда собрались пойти в магазин за молоком.
// Их хочет поймать Стэнсфилд, поэтому нашим товарищам нужно сделать это как можно быстрее. 
// Каково минимальное количество улиц, по которым пройдёт хотя бы один из ребят (либо Матильда, либо Леон, либо оба вместе)?

// Формат ввода
// Первая строка содержит количество вершин n (1 ≤ n ≤ 105), 
// количество ребер m (0 ≤ m ≤ 105) и номера вершин графа leon, matilda, milk, 
// в которых находятся соответственно Леон, Матильда и магазин с молоком.
// Следующие m строк содержат ребра графа. В каждой строке два числа, разделенные пробелом, 
// если между i и j существует ребро. Гарантируется, что в графе нет петель и мультиребер.

// Формат вывода
// Одно число – количество ребер.

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

static const size_t inf = std::numeric_limits<size_t>::max();

class Graph {
public:
    Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV);
    const std::vector<size_t>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;
private:
    size_t vertexCount;
    std::vector< std::vector<size_t> > data;
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
    return vertexCount;
}

void FindDistance(const Graph& G, size_t source, size_t destination, std::vector<size_t>& result) {
    result[source] = 0;
    std::queue<size_t> queue;
    queue.push(source);
    size_t currentV;
    while (!queue.empty()) {
        currentV = queue.front();
        queue.pop();
        for (size_t incidenceV : G.GetNeighbours(currentV)) {
            if (result[incidenceV] == inf) {
                result[incidenceV] = result[currentV] + 1;
                queue.push(incidenceV);
            }
        }
    }
    return;
}

size_t FindPath(const Graph& G, size_t pos_1, size_t pos_2, size_t pos_3) {
    std::vector<size_t> bfs1_res(G.VertexCount(), inf);
    std::vector<size_t> bfs2_res(G.VertexCount(), inf);
    std::vector<size_t> bfs3_res(G.VertexCount(), inf);
    FindDistance(G, pos_1, pos_3, bfs1_res);
    FindDistance(G, pos_2, pos_3, bfs2_res);
    FindDistance(G, pos_3, pos_1, bfs3_res);
    size_t result = inf;
    for (size_t v = 0; v < G.VertexCount(); ++v) {
        size_t sum = bfs1_res[v] + bfs2_res[v] + bfs3_res[v];
        if (sum < result) {
            result = sum;
        }
    }
    return result;
}

int main() {
    size_t n, m, pos_leon, pos_matilda, pos_shop = 0;
    std::cin >> n >> m >> pos_leon >> pos_matilda >> pos_shop;
    Graph G(n);
    size_t firstV, secondV = 0;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> firstV >> secondV;
        G.AddEdge(firstV - 1, secondV - 1);
    }
    std::cout << FindPath(G, pos_leon - 1, pos_matilda - 1, pos_shop - 1) << std::endl;
    return 0;
}
