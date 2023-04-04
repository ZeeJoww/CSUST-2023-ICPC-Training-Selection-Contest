#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
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

void output(const std::vector<int> &a, const std::vector<PII> &e) {
    std::ofstream fout(getPath());
    fout << a.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [u, v] : e) {
        fout << u << ' ' << v << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 2, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a, std::vector<PII> &e) {
    auto map = shuffle_map(a.size());
    for (auto &[u, v] : e) {
        u = map[u], v = map[v];
    }
    output(a, e);
}

constexpr int lim = 1e9;
RandInt rand_d(1, lim);

std::vector<int> get_a(int n) {
    std::vector<int> a(n);
    for (int &x : a) {
        x = rand_d();
    }
    return a;
}

int main() {
    init();

    { // samples
        { // sample #1
            std::vector<int> a(5, 1);
            std::vector<PII> e{ {2,1}, {3,1}, {4,1}, {5,1} };
            output(a, e);
        }
        { // sample #2
            std::vector<int> a{ 100, 1, 60, 60 };
            std::vector<PII> e{ {1,2}, {1,3}, {3,4} };
            output(a, e);
        }
    }

    constexpr int N = 5e5;
    constexpr int n_list0[] = { 2,3,4,5,6,7,8,9, 5000, N };

    // random: different n
    for (int n : n_list0) {
        std::vector<int> a = get_a(n);
        RandInt rand_p(1, n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(1, now - 1)();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    int n = N;

    // random: linked list shaped tree
    for (int c = 1; c <= 3; c+=2) {
        std::vector<int> a = get_a(n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(std::max(1, now-c), now-1)();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    // random: "flower" shaped tree
    for (int t = 1; t <= 3; t+=2) {
        std::vector<int> a = get_a(n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(1, std::min(now-1, t))();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    return 0;
}
