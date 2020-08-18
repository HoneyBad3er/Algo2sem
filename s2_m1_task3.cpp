// C. Районы, кварталы, жилые массивы
// Фрэнку опять прилетел новый заказ. Однако в этот раз город играет по очень странным правилам: 
// все дороги в городе односторонние и связывают только офисы нанимателей перевозчика. 
// Множество офисов, между любыми двумя из которых существует путь, образуют квартал, 
// если нельзя добавить никакие другие, чтобы условие выполнялось. Фрэнку интересно, 
// каково минимальное количество односторонних дорог нужно ещё построить, чтобы весь город стал квраталом.

// Формат ввода
// Первая строка содержит пару целых чисел n и m (1 ≤ n ≤ 105, 0 ≤ m ≤ 105) — количество вершин и рёбер 
// заданного ориентированного графа.
// Следующие m строк содержат описания рёбер, по одному в строке. 
// Каждая из строк содержит по два целых числа a и b (1 ≤ a, b ≤ n) — начало и конец ребра.

// Формат вывода
// Выведите одно число — количество дорог, которые нужно добавить, чтобы весь город стал одним кварталом.

#include <iostream>
#include <vector>
#include <exception>

enum VertexColor {
    WHITE,
    GREY,
    BLACK
};

class Graph {
public:
    Graph(size_t vCount);
    void AddEdge(size_t firstV, size_t secondV);
    const std::vector<size_t>& GetNeighbours(size_t vertex) const;
    Graph GetTranspone() const ;
    const size_t VertexCount() const;
private:
    size_t vertexCount;
    std::vector< std::vector<size_t> > data;
};

Graph::Graph(size_t vCount): vertexCount(vCount), data(vCount) {}

void Graph::AddEdge(size_t firstV, size_t secondV) {
    data[firstV].push_back(secondV);
}

const std::vector<size_t>& Graph::GetNeighbours(size_t vertex) const {
    return data[vertex];
}

Graph Graph::GetTranspone() const {
    Graph transponed(vertexCount);
    for (size_t v = 0; v < vertexCount; ++v) {
        for (size_t u : data[v]) {
            transponed.AddEdge(u, v);
        }
    }
    return transponed;
}

const size_t Graph::VertexCount() const {
    return vertexCount;
}

// Общий ДФС для нормального и транспонированого графа (флаг transpone)
// В случае транспонированого графа data - orderList (список для элементов в порядке топ сорта)
// В случае нормального графа data - componentList (список компонент связности)
void InnerDFS(const Graph& graph, size_t currentV, std::vector<VertexColor>& colors,
              std::vector<size_t>& data, size_t& componentCount, bool transpone = false) {
    
    colors[currentV] = GREY;
    if (transpone) {
        data[currentV] = componentCount;
    }
    for (size_t vertex : graph.GetNeighbours(currentV)) {
        if (colors[vertex] == WHITE) {
            InnerDFS(graph, vertex, colors, data, componentCount, transpone);
        }
    }
    colors[currentV] = BLACK;
    if (!transpone) {
        data.push_back(currentV);
    }
}

size_t FindRoadsCount(const Graph& graph) {
    std::vector<VertexColor> colors(graph.VertexCount(), WHITE);
    std::vector<size_t> componentList(graph.VertexCount());
    std::vector<size_t> orderList;
    size_t componentCount = 0;
    
    for (size_t vertex = 0; vertex < graph.VertexCount(); ++vertex) {
        if (colors[vertex] == WHITE) {
            InnerDFS(graph, vertex, colors, orderList, componentCount);
        }
    }
    
    colors.assign(graph.VertexCount(), WHITE);
    Graph tansponedGraph(graph.GetTranspone());

    for (auto it = orderList.rbegin(); it != orderList.rend(); ++it) {
        if (colors[*it] == WHITE) {
            InnerDFS(tansponedGraph, *it, colors, componentList, componentCount, true);
            ++componentCount;
        }
    }
    
    if (componentCount == 1) {
        return 0;
    }
    
    std::vector<bool> source(componentCount, false);
    std::vector<bool> stock(componentCount, false);

    for (size_t vertex = 0; vertex < graph.VertexCount(); ++vertex) {
        for (size_t currentV : graph.GetNeighbours(vertex)) {
            if (componentList[vertex] != componentList[currentV]) {
                source[componentList[vertex]] = true;
                stock[componentList[currentV]] = true;
            }
        }
    }
    
    size_t non_source = 0;
    size_t non_stock = 0;
    
    for (size_t i = 0; i < componentCount; ++i) {
        non_source += !source[i];
        non_stock += !stock[i];
    }
    
    return non_stock > non_source ? non_stock : non_source;
}

int main() {
    size_t n, m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    size_t firstV, secondV = 0;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> firstV >> secondV;
        graph.AddEdge(firstV - 1, secondV - 1);
    }
    std::cout << FindRoadsCount(graph) << std::endl;
    return 0;
}
