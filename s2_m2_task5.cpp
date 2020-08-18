// Рик решил на день почувствовать себя бизнесменом!
// В городе есть несколько обменников валюты. В рамках данной задачи считаем, 
// что каждый обменник специализируется только на двух валютах и может производить операции только с ними. 
// Возможно, существуют обменники, специализирующиеся на одинаковых парах валют. В каждом обменнике — 
// свой обменный курс: курс обмена A на B — это количество единиц валюты B, выдаваемое за 1 единицу валюты A. 
// Также в каждом обменнике есть комиссия — сумма, которую вы должны заплатить, чтобы производить операцию. 
// Комиссия взимается в той валюте, которую меняет клиент.
// Например, если вы хотите поменять 100 долларов США на русские рубли в обменнике, где курс обмена равен 29.75, 
// а комиссия равна 0.39, вы получите (100 - 0.39) ⋅ 29.75 = 2963.3975 рублей (эх, были времена).
// Вы точно знаете, что в городе используется всего N валют. Пронумеруем их числами 1, 2, …, N. 
// Тогда каждый обменник представляют 6 чисел: целые A и B — номера обмениваемых валют, а также вещественные 
// RAB, CAB, RBA и CBA — обменные курсы и комиссии при переводе из A в B и из B в A соответственно.
// Рик обладает некоторой суммой в валюте S. Он задаётся вопросом, можно ли, после нескольких операций обмена увеличить свой капитал. 
// Конечно, он хочет, чтобы в конце его деньги вновь были в валюте S. Помогите ему ответить на его вопрос. 
// Рик должен всегда должен иметь неотрицательную сумму денег.

// Формат ввода
// Первая строка содержит четыре числа: N — количество валют, M — количество обменников, S — валюта, 
// которой располагает Рик, и V — количество единиц этой валюты. Следующие M строк содержат по 6 чисел, 
// которые задают описание соответствующих обменников в описанном выше формате. Числа разделяются одним или несколькими пробелами. 
// Выполняются ограничения , V — вещественное число, .
// В каждом обменнике курсы обмена и комиссии — вещественные числа, задаваемые с точностью не выше двух знаков после запятой в десятичном представлении, причём .
// Назовём последовательность операций обмена простой, если каждый обменник используется в ней не более одного раза. 
// Вы можете считать, что отношение итоговой суммы Рика к исходной не будет превосходить 104 при любой простой последовательности операций.

// Формат вывода
// Если Рик может увеличить свой капитал, выведите YES, иначе выведите NO.


#include <iostream>
#include <vector>

//_________EDGE_________
class Edge {
public:
    Edge(int A,
         int B,
         double RAB,
         double CAB);
    int getA() const;
    int getB() const;
    double getRAB() const;
    double getCAB() const;
private:
    int A;
    int B;
    double RAB;
    double CAB;
};

Edge::Edge(int A,
           int B,
           double RAB,
           double CAB):
           A(A), B(B), RAB(RAB),CAB(CAB) {}

int Edge::getA() const {
    return A;
}

int Edge::getB() const {
    return B;
}

double Edge::getRAB() const {
    return RAB;
}

double Edge::getCAB() const {
    return CAB;
}

//_________GRAPH_________
class Graph {
public:
    Graph(int vCount, int eCount);
    void AddEdge(int A, int B, double RAB, double CAB);
    const std::vector<Edge>& GetEdges() const;
    const size_t VertexCount() const;
    const size_t EdgesCount() const;
private:
    const int edgesCount;
    const int vertexCount;
    std::vector<Edge> data;
};

Graph::Graph(int vCount, int eCount): vertexCount(vCount),
                                      edgesCount(eCount) {
    data.reserve(eCount);
}

void Graph::AddEdge(int A, int B, double RAB, double CAB) {
    data.push_back( {A, B, RAB, CAB} );
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

bool HasNegCycle(const Graph& graph, int source, double value) {
    std::vector<double> dist(graph.VertexCount(), 0);
    dist[source] = value;
    for (int i = 0; i < graph.VertexCount(); ++i) {
        for (const Edge& edge : graph.GetEdges()) {
            if ((dist[edge.getA()] - edge.getCAB()) * edge.getRAB() > dist[edge.getB()]) {
                dist[edge.getB()] = (dist[edge.getA()] - edge.getCAB()) * edge.getRAB();
            }
        }
    }
    for (const Edge& edge : graph.GetEdges()) {
        if ((dist[edge.getA()] - edge.getCAB()) * edge.getRAB() > dist[edge.getB()]) {
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int currencyQty, converterQty, source =0;
    double value = 0;
    std::cin >> currencyQty >> converterQty >> source >> value;
    Graph graph(currencyQty, converterQty*2);
    int A, B = 0;
    double RAB, CAB, RBA, CBA = 0;
    for (size_t i = 0; i < converterQty; ++i) {
        std::cin >> A >> B >> RAB >> CAB >> RBA >> CBA;
        graph.AddEdge(A - 1, B - 1, RAB, CAB);
        graph.AddEdge(B - 1, A - 1, RBA, CBA);
    }
    if (HasNegCycle(graph, source - 1, value)) {
        std::cout << "YES" << '\n';
    }
    else {
        std::cout << "NO" << '\n';
    }
    return 0;
}
