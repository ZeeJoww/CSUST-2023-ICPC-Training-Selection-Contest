#include <fstream>
#include <random>
#include <vector>
#include <set>

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

template <typename T=int>
class NoDupRandInt {
    std::uniform_int_distribution<T> u;
    std::set<T> except;
public:
    NoDupRandInt(const T &lb, const T &ub) : u(lb, ub) {}
    NoDupRandInt(const T &lb, const T &ub, const std::set<T> &set) : u(lb, ub), except(set) {}
    T operator() () {
        T res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

struct Testcase {
    int a, b, c, d, e, f;
    Testcase() = default;
    Testcase(int _a, int _b, int _c, int _d, int _e, int _f) 
        : a(_a), b(_b), c(_c), d(_d), e(_e), f(_f) {}
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int,int>;

void output(const std::vector<Testcase> &tests) {
    std::ofstream fout(getPath());
    fout << tests.size() << '\n';
    for (auto [a, b, c, d, e, f] : tests) {
        fout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << ' ' << f << '\n';
    }
    fout.close();
}

int main() {
    init();

    constexpr int top_cnt = 1e5, lim = 1e6;

    { // sample
        std::vector<Testcase> tests{
            {1,2,1,2,3,6}, {1,2,1,2,3,3}, {1,1,4,5,1,4}, {1,0,0,2,1,1}
        };
        output(tests);
    }

    { // hand: small && on the same line
        constexpr int times_top(43478);
        RandInt rand_tm(-times_top, times_top);
        std::vector<Testcase> tests;
        int value_list[2*23];
        for (int i = 1; i <= 23; i++) {
            value_list[i*2-2] = +i;
            value_list[i*2-1] = -i;
        }
        for (int i : value_list) {
            for (int j : value_list) {
                for (int k : value_list) {
                    int tm = rand_tm();
                    tests.emplace_back(i, tm*i, j, tm*j, k, tm*k);
                }
            }
        }
        output(tests);
    }

    { // hand: always exists
        RandInt rand_abs(1, 5000), rand_sign(0, 1);
        auto randInt = [&rand_abs, &rand_sign] () -> int {
            return (rand_sign() ? +1 : -1) * rand_abs();
        };
        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests) {
            do {
                int x = randInt(), y = randInt();
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
                e = a * x + c * y, f = b * x + d * y;
            } while (abs(e) + abs(f) == 0 || abs(e) > lim || abs(f) > lim);
        }
        output(tests);
    }

    constexpr int top_list0[] = { 500, lim };

    for (int top : top_list0) { // hand: contains zeros
        RandInt rand_abs(1, top), rand_type(0, 1);
        auto randInt = [&rand_abs, &rand_type] () -> int {
            return (rand_type() ? +1 : -1) * rand_abs();
        };
        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests) {
            a = 0, b = randInt(), c = 0, d = randInt();
            if (rand_type()) std::swap(a, b);
            if (rand_type()) std::swap(c, d);

            do {
                e = randInt(), f = randInt();
                if (rand_type()) {
                    if (rand_type()) e = 0;
                    else f = 0;
                }
            } while (abs(e) + abs(f) == 0);
        }
        output(tests);
    }

    constexpr int top_list[] = { 20, 500, 5000, lim };

    // random with limited upper bound
    for (int top : top_list) {
        RandInt randInt(-top, top);
        auto get_p = [&randInt] (int &a, int &b) -> void {
            do { a = randInt(), b = randInt(); } while (abs(a) + abs(b) == 0);
        };

        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests){
            get_p(a, b), get_p(c, d), get_p(e, f);
        }
        output(tests);
    }

    { // hand: upper bound case
        std::vector<Testcase> tests(top_cnt);
        int n = lim;
        for (auto &[a, b, c, d, e, f] : tests) {
            a = n, b = n - 1, c = n + 1, d = n, e = n, f = -n;
            n -= 2;
        }
        output(tests);
    }

    return 0;
}
