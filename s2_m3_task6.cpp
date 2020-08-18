// Шрек и Фиона пригласили всех своих друзей на свою свадьбу. На церемонии они хотят рассадить их всех на две непустые части так, 
// чтобы количество знакомств между двумя частями было минимальным. Всего приглашенных на свадьбу n, а каждое знакомство обоюдно.
// Вам дан граф, в котором ребро означает знакомство между людьми. Помогите Шреку и Фионе поделить гостей на две непустые части.

// Формат ввода
// В первой строке входного файла записано целое число n () — число гостей. Каждая из следующих n строк содержит по n символов.
// i-й символ j-й из этих строк равен «1», если между вершинами i и j есть ребро, и «0» в противном случае. 
// Заданная таким образом матрица смежности является антирефлексивной (на главной диагонали стоят нули) и симметричной (относительно главной диагонали).

// Формат вывода
// Выведите в выходной файл две строки. На первой выведите номера вершин, попавших в левую часть, через пробел, а на второй — номера вершин, 
// попавших в правую часть, также через пробел. Номера вершин можно выводить в любом порядке.

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

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
    stream << "( " << edge.end + 1 << ", " << edge.oposite << ", " << edge.flow << ") ";
    return stream;
}

//_________GRAPH_________
class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(int begin, int end);
    const std::vector<std::vector<Edge>>& GetEdges() const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector<std::vector<Edge>> edges;
};

Graph::Graph(size_t vCount): vertexCount(vCount), edges(vertexCount) {}

void Graph::AddEdge(int begin, int end) {
    int rPos1 = static_cast<int>(edges[begin].size());
    int rPos2 = static_cast<int>(edges[end].size());
    edges[begin].push_back(Edge(begin, end, rPos2, 0, 1));
    edges[end].push_back(Edge(end, begin, rPos1, 0, 1));
}

const std::vector<std::vector<Edge>>& Graph::GetEdges() const {
    return edges;
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

int InnerDfs(std::vector<std::vector<Edge>>& edges, std::vector<bool>& visited,
             int start, int dest, int flow) {
    
    if (start == dest) {
        return flow;
    }
    visited[start] = true;
    for (int end = 0; end < edges[start].size(); ++end) {
        Edge& edge = edges[start][end];
        Edge& reversedEdge = edges[edge.end][edge.oposite];
        if (edge.cpt > edge.flow && !visited[edge.end]) {
            int curFlow = InnerDfs(edges, visited, edge.end, dest, edge.cpt - edge.flow);
            if (curFlow > 0) {
                edge.flow += curFlow;
                reversedEdge.flow -= curFlow;
                return curFlow;
            }
        }
    }
    return 0;
}

void GetCut(std::vector<std::vector<Edge>>& edges,
        int start, int dest, std::vector<bool>& inLeftHalf) {

    inLeftHalf[start] = true;
    if (start == dest) {
        return;
    }
    for (int end = 0; end < edges[start].size(); ++end) {
        Edge& edge = edges[start][end];
        if (edge.cpt > edge.flow && !inLeftHalf[edge.end]) {
            GetCut(edges, edge.end, dest, inLeftHalf);
        }
    }
}
    
void GetCut(const Graph& graph) {
    std::vector<bool> visited(graph.VertexCount(), false);
    int start = 0;
    int maxFlow = 0;
    int curFlow = 0;
    int globalFlow = std::numeric_limits<int>::max();
    std::vector<bool> inLeftHalf(graph.VertexCount(), false);

    for (int dest = 1; dest < graph.VertexCount(); ++dest) {
        std::vector<std::vector<Edge>> edges = graph.GetEdges();
        maxFlow = 0;
        curFlow = 0;
        while ((curFlow = InnerDfs(edges, visited, start, dest, 1)) > 0) {
            visited.assign(graph.VertexCount(), false);
            maxFlow += curFlow;
        }
        visited.assign(graph.VertexCount(), false);
        if (maxFlow < globalFlow) {
            globalFlow = maxFlow;
            inLeftHalf.assign(graph.VertexCount(), false);
            GetCut(edges, start, dest, inLeftHalf);
        }
    }

    std::stringstream rightHalf;
    for (size_t guest = 0; guest < graph.VertexCount(); ++guest) {
        if (inLeftHalf[guest]) {
            std::cout << guest + 1 << " ";
        }
        else {
            rightHalf << guest + 1 << " ";
        }
    }
    std::cout << "\n" << rightHalf.str();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int vertexCount = 0;
    std::cin >> vertexCount;
    Graph graph(vertexCount);
    std::string row;
    for (int row_num = 0; row_num < vertexCount; ++row_num) {
        std::cin >> row;
        for (int col_num = row_num + 1; col_num < vertexCount; ++col_num) {
            if (row[col_num] == '1') {
                graph.AddEdge(row_num, col_num);
            }
        }
    }
    GetCut(graph);
    return 0;
}
