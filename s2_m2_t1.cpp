// Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше M. 
// Они могут телепортироваться из вселенной с номером z во вселенную (z+1) mod M за a бутылок лимонада или во 
// вселенную (z2+1) mod M за b бутылок лимонада. Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y. 
// Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше?

// Формат ввода
// В строке подряд даны количество бутылок a за первый тип телепортации, количество бутылок b за второй тип телепортации, 
// количество вселенных M, номер стартовой вселенной x, номер конечной вселенной y (0 ≤ a, b ≤ 100, 1 ≤ M ≤ 106, 0 ≤ x, y < M).

// Формат вывода
// Выведите одно число — количество бутылок лимонада, которые отдаст Рик за такое путешествие.


#include <iostream>
#include <utility>
#include <set>
#include <vector>
#include <limits>

const int Max = std::numeric_limits<int>::max();

struct Vertex {
    Vertex() = delete;
    explicit Vertex(int vertex, int dist);
    int vertex;
    int dist;
    friend bool operator < (const Vertex& lhs, const Vertex& rhs);
    friend bool operator == (const Vertex& lhs, const Vertex& rhs);
};

Vertex::Vertex(int vertex, int dist) : vertex(vertex), dist(dist) {}

bool operator < (const Vertex& lhs, const Vertex& rhs) {
    return lhs.dist == rhs.dist ? lhs.vertex < rhs.vertex :
                                  lhs.dist < rhs.dist;
}

bool operator == (const Vertex& lhs, const Vertex& rhs) {
    return lhs.dist == rhs.dist & lhs.vertex == rhs.vertex;
}

int GetMinTravelCost(int cost1, int cost2,
                  int universeQty, int source,
                  int destination) {

    std::vector<int> distance(universeQty, Max);
    std::set<Vertex> vertices;
    vertices.insert(Vertex(source, 0));
    while (!vertices.empty()) {
        int currentV = vertices.begin()->vertex;
        distance[currentV] = vertices.begin()->dist;
        vertices.erase(vertices.begin());
        if (currentV == destination) {
            return distance[destination];
        }
        int vertex1 = (currentV + 1) % universeQty;
        int vertex2 = static_cast<int>((static_cast<long long>(currentV) * currentV + 1)
                                       % universeQty);
        if (distance[currentV] + cost1 < distance[vertex1]) {
            vertices.erase(Vertex(vertex1, distance[vertex1]));
            distance[vertex1] = distance[currentV] + cost1;
            vertices.insert(Vertex(vertex1, distance[vertex1]));
        }
        if (distance[currentV] + cost2 < distance[vertex2]) {
            vertices.erase(Vertex(vertex2, distance[vertex2]));
            distance[vertex2] = distance[currentV] + cost2;
            vertices.insert(Vertex(vertex2, distance[vertex2]));
        }
    }
    return distance[destination];
}

int main() {
    int a, b, M, x, y = 0;
    std::cin >> a >> b >> M >> x >>y;
    std::cout << GetMinTravelCost(a, b, M, x, y) << std::endl;
    return 0;
}
