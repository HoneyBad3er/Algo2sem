// У художника-авангардиста есть полоска разноцветного холста. За один раз он перекрашивает некоторый отрезок полоски в некоторый цвет. 
// После каждого перекрашивания специально обученный фотограф делает снимок части получившегося творения для музея современного искусства. 
// Для правильного экспонирования требуется знать яркость самого темного цвета на выбранном фотографом отрезке. 
// Напишите программу для определения яркости самого темного цвета на отрезке.
// Требуемая скорость определения яркости — O(log N).
// Цвет задается тремя числами R, G и B (0 ≤ R, G, B ≤ 255), его яркость = R + G + B.
// Цвет (R1, G1, B1) темнее цвета (R2, G2, B2), если R1 + G1 + B1 < R2 + G2 + B2.

// Формат ввода
// Первая строка содержит число N –– длина отрезка (координаты [0...N-1]).
// Затем следует N строк по 3 числа –– цвета отрезка.
// Следующая строка содержит число K –– количество перекрашиваний и фотографирований.
// Затем задано K строк, описывающих перекрашивание и фотографирование.
// В каждой такой строке 7 натуральных чисел: C, D, R, G, B, E, F, где [C, D] –– координаты перекрашиваемого отрезка, 
// (R, G, B) –– цвет покраски, [E, F] –– фотографируемый отрезок.

// Формат вывода
// Требуется вывести K целых чисел. Каждое число –– яркость самого темного цвета на отрезке [E, F] 
// после соответствующего окрашивания.

#include <iostream>
#include <vector>
#include <limits>

class TreeSegments {
public:
    TreeSegments(const std::vector<long long>& inp);
    void setOnSubsegment(size_t l, size_t r, long long set_v);
    long long getMinOnSubsegment(size_t l, size_t r);

private:
    struct Node {
        Node() = default;
        explicit Node(long long num, long long delta) : num(num), delta(delta) {}
        long long num;
        long long delta;
    };
    
    size_t n;
    std::vector<Node> tree;
    
    void build(const std::vector<long long>& inp, size_t v, size_t l, size_t r);
    void update(size_t v, size_t l, size_t r);
    void push(size_t v);
    long long getMin(size_t v, size_t l, size_t r, size_t begin, size_t end);
    void set(size_t v, long long set_v, size_t l, size_t r, size_t begin, size_t end);
};

TreeSegments::TreeSegments(const std::vector<long long>& inp) : n(inp.size()), 
                                                                tree(4 * inp.size(), Node(0, -1)) {
    build(inp, 1, 0, inp.size());
}

void TreeSegments::build(const std::vector<long long>& inp, size_t v, size_t l, size_t r) {
    if (r - l == 1) {
        tree[v] = Node(inp[l], -1);
    } 
    else {
        size_t m = l + (r - l) / 2;
        build(inp, 2 * v, l, m);
        build(inp, 2 * v + 1, m, r);
        update(v, l, r);
    }
}

void TreeSegments::update(size_t v, size_t l, size_t r) {
    if (r - l == 1) {
        return;
    }
    tree[v].num = std::min(tree[2 * v].num, tree[2 * v + 1].num);
}

void TreeSegments::push(size_t v) {
    if (tree[v].delta == -1) {
        return;
    }
    size_t left = 2 * v;
    size_t right = 2 * v + 1;
    
    if (left < tree.size()) {
        tree[left].num = tree[v].delta;
        tree[left].delta = tree[v].delta;
    }
    if (right < tree.size()) {
        tree[right].num = tree[v].delta;
        tree[right].delta = tree[v].delta;
    }
    
    tree[v].delta = -1;
}

long long TreeSegments::getMin(size_t v, size_t l, size_t r, size_t begin, size_t end) {
    if (r <= begin || end <= l) {
        return std::numeric_limits<long long>::max();
    }
    
    push(v);
    
    if (begin <= l && r <= end) {
        return tree[v].num;
    }
    
    size_t m = l + (r - l) / 2;
    long long result = std::min(getMin(2 * v, l, m, begin, end), getMin(2 * v + 1, m, r, begin, end));
    update(v, l, r);
    
    return result;
}

void TreeSegments::set(size_t v, long long set_v, size_t l, size_t r, size_t begin, size_t end) {
    if (r <= begin || end <= l) {
        return;
    }
    
    if (begin <= l && r <= end) {
        tree[v].delta = set_v;
        tree[v].num = set_v;
        push(v);
        return;
    }
    
    push(v);
    
    size_t m = l + (r - l) / 2;
    set(2 * v, set_v, l, m, begin, end);
    set(2 * v + 1, set_v, m, r, begin, end);
    update(v, l, r);
}

void TreeSegments::setOnSubsegment(size_t l, size_t r, long long set_v) {
    set(1, set_v, 0, n, l, r);
}

long long TreeSegments::getMinOnSubsegment(size_t l, size_t r) {
    return getMin(1, 0, n, l, r);
}

int main() {
    size_t n;
    std::cin >> n;
    
    std::vector<long long> inp(n);
    for (size_t i = 0; i < n; ++i) {
        size_t r, g, b;
        std::cin >> r >> g >> b;
        inp[i] = r + g + b;
    }

    TreeSegments tree(inp);
    size_t k;
    std::cin >> k;
    
    for (size_t i = 0; i < k; ++i) {
        size_t c, d, r, g, b, e, f;
        std::cin >> c >> d >> r >> g >> b >> e >> f;
        
        size_t color = r + g + b;
        tree.setOnSubsegment(c, d + 1, color);
        std::cout << tree.getMinOnSubsegment(e, f + 1) << " ";
    }

    return 0;
}
