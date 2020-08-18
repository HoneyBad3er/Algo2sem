// B. Секретные материалы
// Джон Макклейн сообщает по рации новую информацию о террористах в отдел с n полицейскими. 
// Он звонит нескольким сотрудникам и просит распространить информацию по отделу, 
// зная, что у каждого полицейского есть связь с определёнными коллегами. 
// Джон Макклейн хочет, чтобы операция прошла успешно. Но если полицейский позвонит коллеге, 
// от которого(возможно, не напрямую) сам получил информацию, террористы смогут отследить этот звонок 
// и помешать операции. Если же двое сотрудников оповестят одного, ничего плохого не произойдёт. 
// Помогите Джону Макклейну. Выведите NO, если кто-то из полицейских ошибётся, делая звонок. 
// Если всё пройдёт хорошо, выведите YES и порядок, в котором полицейские получат информацию,
// считая, что полицейские оповещают коллег по возрастанию их номеров, а в начале Джон даёт информацию тем, 
// кому не может позвонить никто из коллег.

// Формат ввода
// Первая строка содержит пару целых чисел n и m (1 ≤ n, m ≤ 105) — количество вершин и рёбер заданного ориентированного графа.
// Следующие m строк содержат описания рёбер, по одному в строке. 
// Каждая из строк содержит по два целых числа a и b (0 ≤ a, b < n) — начало и конец ребра.

// Формат вывода
// Если такого порядка не существует, то выведите «NO» (без кавычек).
// В противном случае, выведите «YES» в первую из строк. 
// Во вторую строку выведите перестановку индексов вершин (целые числа от 0 до n - 1), 
// такую, что для каждого ребра его стартовая вершина находится строго левее его конечной вершины.

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

const size_t Graph::VertexCount() const {
    return vertexCount;
}

void FindCycle(const Graph& graph, size_t currentV, 
              std::vector<VertexColor>& colors,
              std::vector<size_t>& ringOrder) {
    
    colors[currentV] = GREY;
    for (size_t vertex : graph.GetNeighbours(currentV)) {
        if (colors[vertex] == WHITE) {
            FindCycle(graph, vertex, colors, ringOrder);
        }
        else if (colors[vertex] == GREY) {
            throw std::exception();
        }
    }
    colors[currentV] = BLACK;
    ringOrder.push_back(currentV);
}

void PrintOrder(const std::vector<size_t>& ringOrder) {
    for (auto it = ringOrder.rbegin(); it != ringOrder.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void FindPath(const Graph& graph) {
    std::vector<VertexColor> colors(graph.VertexCount(), WHITE);
    std::vector<size_t> ringOrder;
    bool hasCycle = false;
    for (size_t currentV = 0; currentV < graph.VertexCount(); ++currentV) {
        if (colors[currentV] == WHITE) {
            try {
                FindCycle(graph, currentV, colors, ringOrder);
            }
            catch (std::exception& ex) {
                hasCycle = true;
            }
        }
    }
    
    if (!hasCycle) {
        std::cout << "YES" << std::endl;
        PrintOrder(ringOrder);
    }
    else {
        std::cout << "NO" << std::endl;
    }
}

int main() {
    size_t n, m = 0;
    std::cin >> n >> m;
    Graph graph(n);
    size_t firstV, secondV = 0;
    for (size_t i = 0; i < m; ++i) {
        std::cin >> firstV >> secondV;
        graph.AddEdge(firstV, secondV);
    }
    FindPath(graph);
    return 0;
}
