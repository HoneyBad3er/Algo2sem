// Сейчас Рику надо попасть из вселенной с номером S во вселенную с номером F. 
// Он знает все существующие телепорты, и казалось бы нет никакой проблемы. 
// Но, далеко не секрет, что за свою долгую жизнь Рик поссорился много с кем. 
// Из своего личного опыта он знает, что при телепортациях есть вероятность, что его заставят ответить за свои слова.
// Если Рик знает вероятности быть прижатым к стенке на всех существующих телепортациях, 
// помогите ему посчитать минимальную вероятность, что он всё-таки столкнется с неприятностями.

// Формат ввода
// В первой строке даны количество вселенных N (2 ≤ N ≤ 100), количество существующих телепортаций M (1 ≤ M ≤ N ⋅ (N - 1) / 2), 
// стартовая вселенная S (1 ≤ S ≤ N) и конечная вселенная F (1 ≤ F ≤ N).
// Далее в M строках дается описание телепортаций в формате Si, Fi, Pi, где Si и Fi — номера вселенных, 
// которые связывает i-я телепортация (телепорты двунаправленные, 1 ≤ Si, Fi ≤ N), Pi — вероятность 
// быть избитым на i-й телепортации в процентах (0 ≤ Pi ≤ 100).

// Формат вывода
// Вероятность быть избитым с точностью до 10-6.


#include <iostream>
#include <vector>
#include <utility>
#include <set>

struct Edge {
    Edge() = delete;
    explicit Edge(size_t vertex, size_t cost);
    size_t vertex;
    size_t cost;
};

Edge::Edge(size_t vertex, size_t cost) : vertex(vertex), cost(cost) {}

class Graph {
public:
    explicit Graph(size_t vertexCount);
    void AddEdge(size_t firstV, size_t secondV, double cost);
    const std::vector<Edge>& GetNeighbours(size_t vertex) const;
    const size_t VertexCount() const;
private:
    const size_t vertexCount;
    std::vector< std::vector<Edge> > data;
};

Graph::Graph(size_t vertexCount): vertexCount(vertexCount) {
    data.resize(vertexCount);
}

void Graph::AddEdge(size_t firstV, size_t secondV, double cost) {
    data[firstV].push_back(Edge(secondV, cost));
    data[secondV].push_back(Edge(firstV, cost));
}

const std::vector<Edge>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

double GetMinDistance(const Graph& graph, size_t source, size_t destination) {
    std::vector<double> distance(graph.VertexCount(), 0);
    distance[source] = 1;
    std::set< std::pair<double, size_t> > unvisited;
    unvisited.insert( {1, source} );
    while (!unvisited.empty()) {
        auto it = unvisited.end();
        --it;
        size_t currentV = it->second;
        distance[currentV] = it->first;
        unvisited.erase(it);
        for (const Edge& u : graph.GetNeighbours(currentV)) {
            if (distance[currentV] * u.cost > distance[u.vertex]) {
                unvisited.erase( {distance[u.vertex], u.vertex} );
                distance[u.vertex] = distance[currentV] * u.cost;
                unvisited.insert( {distance[u.vertex], u.vertex} );
            }
        }
    }
    return 1 - distance[destination];
}


int main() {
    size_t vertexCount = 0;
    size_t edgesCount = 0;
    size_t source =0;
    size_t destination = 0;
    std::cin >> vertexCount >> edgesCount >> source >> destination;
    Graph graph(vertexCount);
    size_t begin = 0;
    size_t end = 0; 
    size_t cost = 0;
    for (size_t i = 0; i < edgesCount; ++i) {
        std::cin >> begin >> end >> cost;
        graph.AddEdge(begin - 1, end - 1, (100. - cost)/100.);
    }
    std::cout << GetMinDistance(graph, source - 1, destination - 1) << std::endl;
    return 0;
}
