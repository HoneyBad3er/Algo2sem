// Рику необходимо попасть на межвселенную конференцию. За каждую телепортацию он платит бутылками лимонада, 
// поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!). 
// Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день. 
// Ему известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада), 
// учитывая, что телепортация не занимает времени, а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!

// Формат ввода
// В первой строке находятся количество вселенных N, количество существующих телепортаций M, количество перелетов подряд K, 
// номер вселенной Рика S, номер вселенной конференции F (2 ≤ N ≤ 300, 1 ≤ M ≤ 105, 1 ≤ K ≤ 300, 1 ≤ S ≤ N, 1 ≤ F ≤ N).

// Далее идет M строк, задающих телепортации. i-я строка содержит три натуральных числа: Si, Fi и Pi, где Si - номер вселенной, 
// из которой ведет i-я телепортация, Fi - номер вселенной, в которую ведет i-я телепортация, Pi - стоимость i-й телепортации. 
// 1 ≤ Si ≤ N, 1 ≤ Fi ≤ N, 1 ≤ Pi ≤ 106.

// Формат вывода
// Выведите одно число - минимальную стоимость пути, подходящего для профессора. 
// Если профессор не сможет за K ночей добраться до конференции, выведите число -1.


#include <iostream>
#include <vector>
#include <utility>
#include <set>

const int Max = std::numeric_limits<int>::max();

struct Edge {
    Edge() = delete;
    explicit Edge(size_t vertex, size_t cost);
    size_t vertex;
    size_t cost;
};

Edge::Edge(size_t vertex, size_t cost) : vertex(vertex), cost(cost) {}

class Graph {
public:
    explicit Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV, size_t cost);
    const std::vector<Edge>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector< std::vector<Edge> > data;
};

Graph::Graph(size_t vCount): vertexCount(vCount) {
    data.resize(vCount);
}

void Graph::AddEdge(size_t firstV, size_t secondV, size_t cost) {
    data[firstV].push_back(Edge(secondV, cost));
}

const std::vector<Edge>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

int FormatAns(size_t ans) {
    return ans == Max ? -1 : static_cast<int>(ans);
}

int GetMinDistance(const Graph& graph, size_t maxEdges, size_t source, size_t destination) {
    std::vector<size_t> distance(graph.VertexCount(), Max);
    std::vector<size_t> lenght(graph.VertexCount(), 0);
    distance[source] = 0;
    std::set< std::pair<size_t, size_t> > unvisited;
    unvisited.insert( {0, source} );
    while (!unvisited.empty()) {
        size_t currentV = unvisited.begin()->second;
        size_t currentLen = lenght[currentV] + 1;
        if (distance[currentV] == Max) {
            return FormatAns(distance[destination]);
        }
        distance[currentV] = unvisited.begin()->first;
        unvisited.erase(unvisited.begin());
        for (const Edge& u : graph.GetNeighbours(currentV)) {
            if (distance[currentV] + u.cost < distance[u.vertex] 
                && currentLen <= maxEdges) {

                unvisited.erase( {distance[u.vertex], u.vertex} );
                distance[u.vertex] = distance[currentV] + u.cost;
                lenght[u.vertex] = currentLen;
                
                if (lenght[u.vertex] <= maxEdges) {
                    unvisited.insert( {distance[u.vertex], u.vertex} );
                }
            }
        }
    }
    return FormatAns(distance[destination]);
}


int main() {
    size_t universeQty = 0; 
    size_t edges = 0; 
    size_t maxEdges = 0;
    size_t source = 0;
    size_t destination = 0;
    std::cin >> universeQty >> edges >> maxEdges >> source >> destination;
    Graph graph(universeQty);
    size_t begin = 0;
    size_t end = 0;
    size_t cost = 0;
    for (size_t i = 0; i < edges; ++i) {
        std::cin >> begin >> end >> cost;
        graph.AddEdge(begin - 1, end - 1, cost);
    }
    std::cout << GetMinDistance(graph, maxEdges, source - 1, destination - 1) << std::endl;
    return 0;
}
