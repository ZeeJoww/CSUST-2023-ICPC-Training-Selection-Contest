#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 5e5, lim = 1e9, topcnt = 10000;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int, int>;

struct Testcase {
    std::vector<int> price;
    std::vector<PII> rela;
    Testcase() = default;
    Testcase(const std::vector<int> &_p, const std::vector<PII> &_r) : price(_p), rela(_r) {}
};

void output(std::ostream &fout, const std::vector<int> &price, const std::vector<PII> &rela) {
    fout << price.size() << ' ' << rela.size() << '\n';
    for (const int &x : price) {
        fout << x << " \n"[&x == &*price.rbegin()];
    }
    for (auto [u, v] : rela) {
        fout << u << ' ' << v << '\n';
    }
}

#include <iostream>
void output_all(const std::vector<Testcase> &tests) {
    if (tests.size() > topcnt) {
        std::cerr << "cases num limit exceeded (tests.size() == " << tests.size() << ")\n";
        return;
    }
    int sum_n = 0, sum_m = 0;
    for (const auto &[p, r] : tests) {
        sum_n += p.size();
        sum_m += r.size();
    }
    if (sum_n > N) {
        std::cerr << "n sum limit exceeded (sum of n == " << sum_n << ")\n";
        return;
    }
    if (sum_m > N) {
        std::cerr << "m sum limit exceeded (sum of m == " << sum_m << ")\n";
        return;
    }
    std::ofstream fout(getPath());
    fout << tests.size() << '\n';
    for (const auto &[price, rela] : tests) {
        output(fout, price, rela);
    }
    fout.close();
}

void output_sing(const std::vector<int> &price, const std::vector<PII> &rela) {
    std::vector<Testcase> test { Testcase(price, rela) };
    output_all(test);
}

std::vector<int> getprice(int n, int top) {
    std::vector<int> price(n);
    RandInt rand_p(1, top);
    for (int &p : price) {
        p = rand_p();
    }
    return price;
}

int main() {
    init();

    { // hand cases: sample
        std::vector<int> price{ 3,7,9 };
        std::vector<PII> rela{ {1,2}, {2,3}, {3,1} };
        output_sing(price, rela);
    }

    constexpr int top_list[2] = { 10, lim };

    // hand cases: complete graph
    for (int top : top_list) {
        int n = 707;
        std::vector<int> price = getprice(n, top);
        std::vector<PII> rela;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    rela.emplace_back(i, j);
                }
            }
        }
        output_sing(price, rela);
    }

    // hand cases: dag ensured 
    for (int top : top_list) { 
        std::vector<int> price = getprice(N, top);
        std::vector<PII> rela(N);
        std::set<PII> has;
        RandInt randInt(1, N);
        for (auto &[u, v] : rela) {
            do {
                u = randInt(), v = randInt();
                if (u > v) std::swap(u, v);
            } while (u == v || has.count(std::make_pair(u, v)));
            has.emplace(u, v);
        }
        output_sing(price, rela);
    }

    // hand cases: dag layers 
    for (int top : top_list) { 
        std::vector<int> price = getprice(N, top);
        std::vector<PII> rela;
        int layers = 500, sz = 1000;
        for (int i = 1; i < layers; i++) {
            RandInt randInt(1, i * sz);
            std::set<PII> has;
            for (int j = 0; j < 4*i; j++) {
                int u, v;
                do {
                    u = i * sz + randInt(), v = randInt();
                    if (u > v) std::swap(u, v);
                } while (u == v || has.count(std::make_pair(u, v)));
                has.emplace(u, v);
                rela.emplace_back(u, v);
            }
        }
        output_sing(price, rela);
    }

    // hand cases: intentionally copied 
    for (int i = 1; i <= 2; i++) {
        std::string filename = "./special-in-" + std::to_string(i) + ".in";
        std::ifstream fin(filename);
        int n, m;
        fin >> n >> m;
        std::vector<int> price = getprice(n, 100);
        std::vector<PII> rela(m);
        for (auto &[u, v] : rela) {
            fin >> v >> u;
        }
        fin.close();
        output_sing(price, rela);
    }

    constexpr PII nm_list[] = { {50,50}, {N,N}, {50,1000}, {1000,50} };

    // random cases
    for (auto [n, m] : nm_list) {
        for (int top : top_list) {
            std::vector<Testcase> tests(N / std::max(n, m));
            for (auto &[price, rela] : tests) {
                price = getprice(n, top);
                rela = std::vector<PII>(m);
                RandInt randInt(1, n);
                for (auto &[u, v] : rela) {
                    do { u = randInt(), v = randInt(); } while (u == v);
                }
            }
            output_all(tests);
        }
    }

    return 0;
}
