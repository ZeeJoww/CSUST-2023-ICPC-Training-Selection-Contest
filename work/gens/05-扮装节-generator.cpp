#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int max_weight = 1e6;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

class NoDupRandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
    std::set<int> except;
public:
    NoDupRandInt(int lb, int ub) : u(lb, ub) {}
    NoDupRandInt(int lb, int ub, const std::set<int> &set) : u(lb, ub), except(set) {}
    int operator() () {
        int res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Edge {
    int u, v, w;
};

struct Query {
    int x, y;
};

void output(int n, int m, int q, auto edges, auto querys) {
    std::ofstream fout(getPath());
    fout << n << ' ' << m << ' ' << q << '\n';
    for (auto [u, v, w] : edges) {
        fout << u << ' ' << v << ' ' << w << '\n';
    }
    for (auto [x, y] : querys) {
        fout << x << ' ' << y << '\n';
    }
    fout.close();
}

std::vector<Edge> getEdges(int m, RandInt &randInt) {
    NoDupRandInt randWeight(1, max_weight);
    std::vector<Edge> edges(m);
    std::set<std::pair<int,int>> has;
    for (auto &[u, v, w] : edges) {
        do {
            u = randInt(), v = randInt(), w = randWeight();
        } while (has.count(std::make_pair(u, v)));
        has.emplace(u, v);
    }
    return edges;
}

std::vector<Query> getQuerys(int q, RandInt &randInt) {
    std::vector<Query> querys(q);
    std::set<std::pair<int,int>> has;
    for (auto &[x, y] : querys) {
        do {
            x = randInt(), y = randInt();
        } while (has.count(std::make_pair(x, y)));
        has.emplace(x, y);
    }
    return querys;
}

void random_free(int n, int m, int q) {
    RandInt randInt(1, n);
    auto edges = getEdges(m, randInt);
    auto querys = getQuerys(q, randInt);
    output(n, m, q, edges, querys);
}

int main() {
    init();

    { // sample
        int n = 2, m = 2, q = 1;
        std::vector<Edge> edges { {2,1,3}, {2,1,6} };
        std::vector<Query> querys { {2,1} };
        output(n, m, q, edges, querys);
    }

    constexpr int N(600), M(1000), Q(100000);

    { // hand
        int n = 501, m = M, q = Q;
        std::vector<Edge> edges;
        RandInt randWeight(1, max_weight);
        for (int i = 2; i <= n; i++) {
            edges.push_back( Edge{1, i, randWeight()} );
            edges.push_back( Edge{i, 1, randWeight()} );
        }
        RandInt randInt(1, n);
        auto querys = getQuerys(q, randInt);
        output(n, m, q, edges, querys);
    }

    // random 
    for (int i = 0; i < 8; i++) {
        random_free(N, M, Q);
    }

    return 0;
}
