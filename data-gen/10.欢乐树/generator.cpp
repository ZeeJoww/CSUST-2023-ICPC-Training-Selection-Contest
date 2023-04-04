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

struct OP {
    int op, pos, d=-1;
    OP() = default;
    OP(int _op, int _pos, int _d=-1) : op(_op), pos(_pos), d(_d) {}
};

using PII = std::pair<int, int>;

void output(const std::vector<int> &a, const std::vector<PII> &e, const std::vector<OP> &q) {
    std::ofstream fout(getPath());
    fout << a.size() << ' ' << q.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [u, v] : e) {
        fout << u << ' ' << v << '\n';
    }
    for (auto [op, p, d] : q) {
        if (op == 1) fout << op << ' ' << p << ' ' << d << '\n';
        else fout << op << ' ' << p << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 2, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a, std::vector<PII> &e, std::vector<OP> &q) {
    auto map = shuffle_map(a.size());
    for (auto &[u, v] : e) u = map[u], v = map[v];
    for (auto &[_1, p, _2] : q) p = map[p];
    output(a, e, q);
}

constexpr int lim = 1e9;
RandInt rand_op(1, 2), rand_d(-lim, lim);

std::vector<int> get_a(int n) {
    std::vector<int> a(n);
    for (int &x : a) {
        x = rand_d();
    }
    return a;
}

std::vector<OP> get_op(int q, RandInt<int> &rand_p) {
    std::vector<OP> opr(q);
    for (auto &[op, p, d] : opr) {
        op = rand_op(), p = rand_p();
        if (op == 2) d = rand_d();
    }
    return opr;
}

int main() {
    init();

    { // samples
        std::vector<int> a{7,3,7,8,0};
        std::vector<PII> e{ {1,2}, {1,3}, {3,4}, {3,5}, {2,3} };
        std::vector<OP> op{ {1,1,-10}, {2,3}, {1,3,20}, {2,3}, {2,5} };
        output(a, e, op);
    }

    constexpr int N = 1e5;
    constexpr int n_list0[] = { 2,3,4,5,6,7,8,9, 100, 5000, N, N };

    // random: different n
    for (int n : n_list0) {
        std::vector<int> p(n + 1), a = get_a(n);
        RandInt rand_p(1, n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            p[now] = RandInt(1, now - 1)();
            e.emplace_back(now, p[now]);
        }
        auto ops = get_op(N, rand_p);
        shuffle_output(a, e, ops);
    }

    int n = N;

    // random: linked list shaped tree
    for (int c = 1; c <= 3; c++) {
        int tp_list[2] = { c, n };
        for (int tp : tp_list) {
            std::vector<int> p(n+1), a = get_a(n);
            RandInt rand_p(1, tp);
            std::vector<PII> e;
            for (int now = 2; now <= n; now ++) {
                p[now] = RandInt(std::max(1, now-c), now-1)();
                e.emplace_back(now, p[now]);
            }
            auto ops = get_op(N, rand_p);
            shuffle_output(a, e, ops);
        }
    }

    // random: linked list shaped tree
    for (int c = 1; c <= 3; c++) {
        std::vector<int> p(n+1), a = get_a(n);
        RandInt rand_p(n-10, n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            p[now] = RandInt(std::max(1, now-c), now-1)();
            e.emplace_back(now, p[now]);
        }
        auto ops = get_op(N, rand_p);
        shuffle_output(a, e, ops);
    }

    // random: "flower" shaped tree
    for (int t = 1; t <= 3; t++) {
        int tp_list[2] = { t, n };
        for (int tp : tp_list) {
            std::vector<int> p(n+1), a = get_a(n);
            RandInt rand_p(1, tp);
            std::vector<PII> e;
            for (int now = 2; now <= n; now ++) {
                p[now] = RandInt(1, std::min(now-1, t))();
                e.emplace_back(now, p[now]);
            }
            auto ops = get_op(N, rand_p);
            shuffle_output(a, e, ops);
        }
    }

    return 0;
}
