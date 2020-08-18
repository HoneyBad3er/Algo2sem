// Шрек и Осёл уже были на пути домой. Им оставалось только преодолеть лес, который отделял их от болота. Но они поссорились, поэтому не хотят идти вместе.
// Лес представляет собой опушки, пронумерованы числами от 1 до n и соединенные m дорожками 
// (может быть несколько дорожек соединяющих две опушки, могут быть дорожки, соединяющие опушку с собой же). 
// Из-за ссоры, если по дорожке прошел один из друзей, то второй по той же дорожке уже идти не может. 
// Сейчас друзья находятся на опушке с номером s, а болото Шрека — на опушке с номером t. Помогите Шреку и Ослу добраться до болота.

// Формат ввода
// В первой строке файла записаны четыре целых числа — n, m, s и t (2 ≤ n ≤ 105, 0 ≤ m ≤ 105, ). В следующих m строках записаны пары чисел. 
// Пара чисел (x, y) означает, что есть дорожка с опушки x до опушки y (из-за особенностей местности дорожки односторонние).

// Формат вывода
// Если существует решение, то выведите YES и на двух отдельных строчках сначала последовательность опушек для Шрека, затем путь для Осла.
// Если решения не существует, выведите NO.
// Если решений несколько, выведите любое.

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>

//_________EDGE_________
struct Edge {
    explicit Edge(int begin, int end, int oposite, int flow, int cpt);
    int begin, end;
    int oposite;
    int flow, cpt;
};

Edge::Edge(int begin, int end, int oposite, int flow, int cpt) : begin(begin), end(end),
                                                                 oposite(oposite), flow(flow), cpt(cpt) {}

std::ostream& operator << (std::ostream& stream, const Edge& edge) {
    stream << edge.end + 1 << " ";
    return stream;
}

//_________GRAPH_________
class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(int begin, int end);
    const std::vector< std::vector<Edge> >& GetEdges() const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector< std::vector<Edge> > edges;
};

Graph::Graph(size_t vCount) : vertexCount(vCount), edges(vertexCount) {}

void Graph::AddEdge(int begin, int end) {
    int rPos1 = static_cast<int>(edges[begin].size());
    int rPos2 = static_cast<int>(edges[end].size());
    edges[begin].push_back(Edge(begin, end, rPos2, 0, 1));
    edges[end].push_back(Edge(end, begin, rPos1, 0, 0));
}

const std::vector< std::vector<Edge> >& Graph::GetEdges() const {
    return edges;
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

bool InnerDfs(std::vector< std::vector<Edge> >& edges, std::vector<bool>& visited,
             int start, int dest) {
    
    if (start == dest) {
        return true;
    }
    visited[start] = true;
    for (int end = 0; end < edges[start].size(); ++end) {
        Edge& edge = edges[start][end];
        Edge& reversedEdge = edges[edge.end][edge.oposite];
        if (edge.cpt > edge.flow && !visited[edge.end]) {
            bool curFlow = InnerDfs(edges, visited, edge.end, dest);
            if (curFlow) {
                edge.flow += 1;
                reversedEdge.flow -= 1;
                return true;
            }
        }
    }
    return false;
}

bool GetPath(std::vector< std::vector<Edge> >& edges,
        int start, int dest, std::vector<bool>& visited, std::vector<Edge>& path) {

    visited[start] = true;
    if (start == dest) {
        return true;
    }
    for (int end = 0; end < edges[start].size(); ++end) {
        Edge& edge = edges[start][end];
        Edge& reversedEdge = edges[edge.end][edge.oposite];
        if (edge.flow == 1 && !visited[edge.end]
            && GetPath(edges, edge.end, dest, visited, path)) {
    
            edge.flow = 0;
            reversedEdge.flow = 0;
            path.push_back(edge);
            return true;
        }
    }
    return false;
}
    
void HasDisjointPath(const Graph& graph, int start, int destination, size_t k) {
    std::vector<bool> visited(graph.VertexCount(), false);
    std::vector< std::vector<Edge> > edges = graph.GetEdges();
    bool hasFlow = true;

    for (size_t i = 0; i < k; ++i) {
        visited.assign(graph.VertexCount(), false);
        hasFlow &= InnerDfs(edges, visited, start, destination);
    }

    if (hasFlow) {
        std::cout << "YES" << "\n";
        std::vector<Edge> path;
        for (size_t i = 0; i < k; ++i) {
            visited.assign(graph.VertexCount(), false);
            GetPath(edges, start, destination, visited, path);
            std::cout << start + 1 << " ";
            for (auto it = path.rbegin(); it != path.rend(); ++it) {
                std::cout << *it;
            }
            std::cout << "\n";
            path.clear();
        }
    }
    else {
        std::cout << "NO" << "\n";
    }
}
    
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t vertexCount, edgesCount = 0;
    int start, destination = 0;
    std::cin >> vertexCount >> edgesCount >> start >> destination;
    Graph graph(vertexCount);
    int begin, end = 0;
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> begin >> end;
        graph.AddEdge(begin - 1, end - 1);
    }
    HasDisjointPath(graph, start - 1, destination - 1, 2);
    return 0;
}