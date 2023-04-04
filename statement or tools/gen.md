```cpp
//=======================
//01-入场券-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 1e7;

std::vector<int> primes;
bool notPrime[N+1];

void init0() {
    for (int i = 2; i <= N; i++) {
        if (notPrime[i]) continue;
        primes.push_back(i);
        for (int j = i*2; j <= N; j += i) {
            notPrime[j] = true;
        }
    }
}

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
    init0();
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

void output(const std::vector<int> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const int &x : vec) {
        fout << x << " \n"[&x == &*vec.rbegin()];
    }
    fout.close();
}

void hand_include(int n, auto iter) {
    std::vector<int> hand(iter, iter + n);
    output(hand);
}

void random(int n, auto randInt) {
    std::vector<int> res(n);
    for (int &x : res) {
        x = randInt();
    }
    output(res);
}

void random_free(int n, int lb, int ub) {
    random(n, NoDupRandInt(lb, ub));
}

void random_except(int n, int lb, int ub, const std::vector<int> &e0) {
    random(n, NoDupRandInt(lb, ub, std::set<int>(e0.begin(), e0.end())));
}

void random_from(int n, const std::vector<int> &from) {
    NoDupRandInt randInt(0, from.size() - 1);
    std::vector<int> res(n);
    for (int &x : res) {
        x = from[randInt()];
    }
    output(res);
}

int main() {
    init();

    constexpr int maxn = 1e5;

    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    hand_include(5, vec.begin());

    hand_include(maxn, primes.begin());
    hand_include(maxn, primes.rbegin());
    hand_include(maxn, primes.begin() + (primes.end() - primes.begin()));

    /* tmp */ {
        std::vector<int> tmp;
        for (int i = N, c = 0; i >= 0; i--) {
            if (notPrime[i]) {
                tmp.push_back(i);
                c++;
                if (c == maxn) {
                    break;
                }
            }
        }
        hand_include(maxn, tmp.begin());
    }

    for (int i = 0; i < 5; i++) {
        random_free(maxn, 1, N);
    }
    for (int i = 0; i < 5; i++) {
        random_except(maxn, 1, N, primes);
    }
    for (int i = 0; i < 5; i++) {
        random_from(maxn, primes);
    }

    for (int i = 1; i <= 5; i++) {
        std::vector<int> vec(maxn);
        RandInt randInt(N - (1 << i), N);
        for (int &x : vec) {
            x = randInt();
        }
        output(vec);
    }

    for (int i = 1; i <= 5; i++) {
        std::vector<int> vec(maxn);
        RandInt randInt(primes.size() - (1 << i), primes.size());
        for (int &x : vec) {
            x = primes[randInt() - 1];
        }
        output(vec);
    }

    return 0;
}
//=======================
//02-副本侠-generator.cpp
#include <fstream>
#include <random>
#include <vector>

constexpr int N = 1e6, topcnt = 10000;

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

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

RandInt getp(0, 100);

struct Testcase {
    int n, p1, p2;
    Testcase(int _n, int _p1, int _p2) : n(_n), p1(_p1), p2(_p2) {}
    Testcase(int _n) : n(_n) {
        do {
            p1 = getp(), p2 = getp();
        } while (p1 + p2 == 0);
    }
};

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &[n, p1, p2] : vec) {
        fout << n << ' ' << p1 << ' ' << p2 << '\n';
    }
    fout.close();
}

void random_free(int lb, int ub) {
    RandInt getn(lb, ub);
    int sum_n = 0, cnt = 0;
    std::vector<Testcase> res;
    while (true) {
        int n = getn();
        sum_n += n;
        cnt ++;
        if (cnt > topcnt || sum_n > N) break;
        res.emplace_back(n);
    }
    output(res);
}

int main() {
    init();

    { // hand cases: sample
        std::vector<Testcase> tmp;
        tmp.emplace_back(1, 50, 50);
        tmp.emplace_back(5, 0, 100);
        output(tmp);
    }

    { // hand cases: all cases with a zero
        std::vector<Testcase> tmp;
        for (int i = 1; i <= 100; i++) {
            tmp.emplace_back(5000, i, 0);
            tmp.emplace_back(5000, 0, i);
        }
        output(tmp);
    }

    { // hand cases: all cases without any zero
        std::vector<Testcase> tmp;
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                tmp.emplace_back(10, i, j);
            }
        }
        output(tmp);
    }

    // random cases: n is tiny
    for (int i = 0; i < 3; i++) {
        random_free(1, 50);
    }

    // random cases: n is normal
    for (int i = 0; i < 3; i++) {
        random_free(50, 1000);
    }

    // random cases: n is large
    for (int i = 0; i < 3; i++) {
        random_free(1000, 10000);
    }

    // random cases: n == N
    for (int i = 0; i < 3; i++) {
        random_free(N, N);
    }

    return 0;
}
//=======================
//03-双子牌-generator.cpp
#include <fstream>
#include <random>
#include <vector>

constexpr int N = 1e6, lim = 1e9, topcnt = 10000;

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

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Card {
    int a, b;
};

typedef std::vector<Card> Testcase;

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &cards : vec) {
        fout << cards.size() << '\n';
        for (const auto &[a, b] : cards) {
            fout << a << ' ' << b << '\n';
        }
    }
    fout.close();
}

void random_free(int lb, int ub, int min, int max) {
    RandInt getn(lb, ub), randInt(min, max);
    int sum_n = 0, cnt = 0;
    std::vector<Testcase> res;
    while (true) {
        int n = getn();
        cnt ++;
        if (n + sum_n > N) {
            n = N - sum_n;
        }
        sum_n += n;
        Testcase cards(n);
        for (auto &[a, b] : cards) {
            do {
                a = randInt(), b = randInt();
            } while (a == b);
        }
        res.emplace_back(cards);
        if (cnt == topcnt || sum_n == N) break;
    }
    output(res);
}

int main() {
    init();

    { // hand cases: sample
        std::vector<Testcase> tmp;
        tmp.emplace_back(Testcase{ {3,5}, {5,7} });
        tmp.emplace_back(Testcase{ {114514,114514}, {114514,233333} });
        output(tmp);
    }

    { // hand cases: containing same
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(1, 1000);
        for (auto &vec : tmp) {
            for (auto &[a, b] : vec) {
                do {
                    a = randInt(), b = randInt();
                } while (a == b);
            }
            vec[randInt() - 1] = ({ int a, b; do {
                    a = randInt(), b = randInt();
                } while (a == b); Card{a, b};
            });
        }
        output(tmp);
    }

    { // hand cases: only 1 solution
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(100, 1000);
        for (auto &vec : tmp) {
            int a0, b0;
            do {
                a0 = randInt(), b0 = randInt();
            } while (a0 == b0);
            for (auto &[a, b] : vec) {
                a = a0, b = b0;
                std::swap(a0, b0);
            }
        }
        output(tmp);
    }

    { // hand cases: specified the differing number
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(1, 10*N-1);
        std::vector<Card> pool;
        for (auto &vec : tmp) {
            pool.push_back( Card {randInt(), randInt()} );
            RandInt choose(0, pool.size() - 1);
            for (auto &card : vec) {
                card = pool[choose()];
            }
        }
        output(tmp);
    }

    // random cases: smaller
    random_free(50, 150, 1, lim);

    // random cases: larger
    random_free(5e4, 1e5, 1, lim);

    // random cases: only one testcase
    random_free(N, N, 1, lim);

    return 0;
}
//=======================
//04-彩虹猫-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int T = 10000, lim = 1e9;

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

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Testcase {
    int x1, y1, x2, y2;
};

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &[x1, y1, x2, y2] : vec) {
        fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';
    }
    fout.close();
}

int main() {
    init();

    { // hand: sample
        std::vector<Testcase> tmp { {4,3,9,9}, {1,2,3,4}, {3,3,3,2} };
        output(tmp);
    }

    { // hand: small & YUUMI
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, 1000);
        std::set<std::pair<int, int>> has;
        for (auto &[a, b, c, d] : tmp) {
            while (true) {
                int x = randInt(), y = randInt();
                if (x == y) continue;
                if (x > y) std::swap(x, y);
                if (has.count(std::pair<int,int>(x,y))) continue;
                has.emplace(x, y);
                a = b = x, c = d = y;
                break;
            }
        }
        output(tmp);
    }

    { // hand: small & GG
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, 1000);
        for (auto &[a, b, c, d] : tmp) {
            do {
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
            } while (c - a == d - b && c - a > 0);
        }
        output(tmp);
    }

    { // hand: larger & YUUMI
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, lim);
        std::set<std::pair<int, int>> has;
        for (auto &[a, b, c, d] : tmp) {
            while (true) {
                int x = randInt(), y = randInt();
                if (x == y) continue;
                if (x > y) std::swap(x, y);
                if (has.count(std::pair<int,int>(x,y))) continue;
                has.emplace(x, y);
                a = b = x, c = d = y;
                break;
            }
        }
        output(tmp);
    }

    { // random
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, lim);
        for (auto &[a, b, c, d] : tmp) {
            do {
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
            } while (a == c && b == d);
        }
        output(tmp);
    }

    return 0;
}
//=======================
//05-扮装节-generator.cpp
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
//=======================
//06-招聘书-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 3000;
using int64 = long long;

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

template <typename T>
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

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

void outputVector(std::ofstream &fout, const auto &vec) {
    for (const auto &x : vec) {
        fout << x << " \n"[&x == &*vec.rbegin()];
    }
}

void output(int n, int m, int k, const auto &v1, const auto &v2, const auto &v3) {
    std::ofstream fout(getPath());
    fout << n << ' ' << m << ' ' << k << '\n';
    outputVector(fout, v1);
    outputVector(fout, v2);
    outputVector(fout, v3);
    fout.close();
}

void random_free(int n, int m, int k, int dtp, int ftp, int64 ttp) {
    std::vector<int> d(n), f(n);
    std::vector<int64> t(n);
    RandInt<int> rand_d(1, dtp), rand_f(1, ftp);
    RandInt<int64> rand_t(1LL, ttp);
    for (int i = 0; i < n; i++) {
        d[i] = rand_d(), f[i] = rand_f(), t[i] = rand_t();
    }
    output(n, m, k, d, f, t);
}

int main() {
    init();

    { // sample
        int n = 1, m = 2, k = 3;
        std::vector<int> df { 1,2,3 };
        std::vector<int64> t { 1LL,2LL,3LL };
        output(n, m, k, df, df, t);
    }

    int64 t_top = int64(1e12);

    { // hand: full
        int n = N, m = N, k = 1;
        std::vector<int> d(n), f(n, 1);
        std::iota(d.begin(), d.end(), 1);
        std::vector<int64> t(n);
        RandInt<int64> rand_t(1LL, t_top);
        for (int64 &x : t) {
            x = rand_t();
        }
        output(n, m, k, d, f, t);
    }

    // random
    constexpr int m_list[] = { 30, 300, 3000 };
    constexpr int k_list[] = { 30, 300, 3000 };
    constexpr int d_list[] = { 30, 300, 3000 };
    constexpr int f_list[] = { 10, 300 };

    for (int m : m_list) {
        for (int k : k_list) {
            for (int d : d_list) {
                for (int f : f_list) {
                    random_free(N, m, k, d, f, t_top);
                }
            }
        }
    }
    random_free(N, N, N, N, N, t_top);

    return 0;
}
//=======================
//07-捏脸师-generator.cpp
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
//=======================
//08-排队论-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

using int64 = long long;

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

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int,int>;

void output(const std::vector<int> &height, const std::vector<PII> &query) {
    std::ofstream fout(getPath());
    fout << height.size() << '\n';
    for (const int &x : height) {
        fout << x << " \n"[&x == &*height.rbegin()];
    }
    for (auto [p, k] : query) {
        fout << p << ' ' << k << '\n';
    }
    fout.close();
}

int main() {
    init();

    constexpr int N = 1e5, lim = 1e9;

    { // sample
        std::vector<int> height { 1,9,1,9,8,10 };
        std::vector<PII> query { {1,1}, {4,5}, {1,4}, {3,7}, {5,0} };
        output(height, query);
    }

    { // hand: same height
        std::vector<int> height(N, lim);
        std::vector<PII> query(N);
        RandInt rand_pos(1, N), choose(0, 4);
        constexpr int GG = lim / 4;
        for (auto &[p, k] : query) {
            p = rand_pos();
            k = GG * choose();
        }
        output(height, query);
    }

    { // hand: very very long
        std::vector<int> height(N);
        RandInt randHigh(lim / 2, lim), randShort(1, 100);
        for (int i = 0; i < N / 10; i++) {
            height[i] = randHigh();
        }
        for (int i = N / 10; i < N; i++) {
            height[i] = randShort();
        }
        std::vector<PII> query(N);
        RandInt rand_pos(1, N), rand_logk(0, 29);
        for (auto &[p, k] : query) {
            p = rand_pos();
            k = (1 << rand_logk()) - 1;
        }
        output(height, query);
    }

    constexpr int h_lim_list[] = { 2, 10, 100, 10000, lim / 100, lim };

    for (int h_lim : h_lim_list) {
        std::vector<int> height(N);
        std::vector<PII> query(N);
        RandInt rand_h(1, h_lim), rand_pos(1, N), rand_k(0, h_lim);
        for (int &h : height) {
            h = rand_h();
        }
        for (auto &[p, k] : query) {
            p = rand_pos(), k = rand_k();
        }
        output(height, query);
    }

    return 0;
}
//=======================
//09-整向量-generator.cpp
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
            value_list[i*2-2] = i;
            value_list[i*2-1] = i;
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

    return 0;
}
//=======================
//10-欢乐树-generator.cpp
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
//=======================
//11-签到题-generator.cpp
#include <fstream>
#include <random>
#include <vector>

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

void output(const std::vector<int> &a) {
    std::ofstream fout(getPath());
    fout << a.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    fout.close();
}

int main() {
    init();

    constexpr int lim = 1e9;

    { // samples
        output(std::vector<int>{1,1});
        output(std::vector<int>{23333,114514,5201314});
        output(std::vector<int>{1,1,1,1});
    }

    // random: small n
    for (int n = 2; n <= 10; n++) {
        constexpr int top_list[] = { 2, 1000, lim / 1000, lim };
        for (int top : top_list) {
            std::vector<int> tmp(n);
            RandInt randInt(1, top);
            for (int &x : tmp) {
                x = randInt();
            }
            output(tmp);
        }
    }

    constexpr int N = 5e5;

    // random: small & large values
    constexpr std::pair<int,int> bt_list[] = { {1,2}, {1,50}, {1,500}, {1,lim}, {lim/2,lim} };

    for (auto [bot, top] : bt_list) {
        std::vector<int> tmp(N);
        RandInt randInt(bot, top);
        for (int &x : tmp) {
            x = randInt();
        }
        output(tmp);
    }

    return 0;
}
//=======================
//12-金币树-generator.cpp
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
//=======================
//13-金手帕-generator.cpp
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

void output(const std::vector<int> &a, const std::vector<PII> &ask) {
    std::ofstream fout(getPath());
    fout << a.size() << ' ' << ask.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [p, k] : ask) {
        fout << p << ' ' << k << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 1, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a0, std::vector<PII> ask) {
    int n = a0.size();
    auto map = shuffle_map(n);
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[map[i+1]] = map[a0[i]];
    }
    for (auto [p, _] : ask) {
        p = map[p];
    }
    output(a, ask);
}

int main() {

    constexpr int N = 1e5, lim = 1e9;

    RandInt rand_k(1, lim);

    { // sample
        std::vector<int> a{ 2,3,1,1,6,5 };
        std::vector<PII> ask{ {3,5}, {4,2}, {5,999999} };
        output(a, ask);
    }

    { // hand: big cycle
        std::vector<int> a(N);
        std::iota(a.begin(), a.end(), 2);
        *a.rbegin() = 1;
        int c0 = 1, k0 = N / 2;
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = c0++, k = k0;
        }
        output(a, ask);
        shuffle_output(a, ask);
    }

    { // hand: self-cycle
        std::vector<int> a(N);
        std::iota(a.begin(), a.end(), 1);
        int c0 = 1;
        RandInt rand_k(1, lim);
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = c0++, k = rand_k();
        }
        output(a, ask);
    }

    // random 1
    for (int i = 0; i < 3; i++) {
        std::vector<int> a(N);
        RandInt rand_p(1, N), rand_k(1, lim);
        for (int &x : a) {
            x = rand_p();
        }
        std::vector<PII> ask(N);
        int c0 = 1;
        for (auto &[p, k] : ask) {
            p = c0++, k = rand_k();
        }
        output(a, ask);
    }

    // random 2
    for (int i = 0; i < 3; i++) {
        std::vector<int> a(N);
        RandInt rand_p(1, N), rand_k(1, lim);
        for (int &x : a) {
            x = rand_p();
        }
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = rand_p(), k = rand_k();
        }
        output(a, ask);
    }

    return 0;
}
//=======================
//14-魔法棒-generator.cpp
#include <fstream>
#include <random>
#include <vector>

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

using RandChar = RandInt<char>;

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct OPR {
    int op_type, x, y;
    OPR() = default;
    std::string to_string() const {
        if (op_type == 0) {
            return "change " + std::to_string(x) + " " + char(y);
        } else {
            return "count-lzl " + std::to_string(x) + " " + std::to_string(y);
        }
    }
};

void output(const std::string &s, const std::vector<OPR> &op) {
    std::ofstream fout(getPath());
    fout << s << '\n';
    fout << op.size() << '\n';
    for (const auto &item : op) {
        fout << item.to_string() << '\n';
    }
    fout.close();
}

RandInt rand_op(0, 1);

template <typename T>
void random_free(int n, int m, T rand_c) {
    std::string s(n, '\0');
    for (char &c : s) {
        c = rand_c();
    }
    std::vector<OPR> op(m);
    RandInt rand_p(1, n);
    for (auto &[op, x, y] : op) {
        op = rand_op();
        if (op == 0) {
            x = rand_p(), y = rand_c();
        } else {
            x = rand_p(), y = rand_p();
            if (x > y) std::swap(x, y);
        }
    }
    output(s, op);
}

int main() {

    constexpr int N = 2e5;

    // tools
    RandInt c2(0, 1), c4(0, 3);
    RandChar rand_c('a', 'z');
    auto rand_lz = [&c2] () -> char {
        constexpr char chrs[2] = { 'l', 'z' };
        return chrs[c2()];
    };
    auto rand_ablz = [&c4] () -> char {
        constexpr char chrs[4] = { 'a', 'b', 'l', 'z' };
        return chrs[c4()];
    };

    { // sample
        std::string s = "lllll";
        std::vector<OPR> op = { {0,3,'z'}, {1,2,4}, {1,1,5}, {0,3,'x'}, {1,1,5} };
        output(s, op);
    }

    { // hand: large answer
        std::string s(N, 'l');
        for (int i = N / 3; i < N * 2 / 3; i++) {
            s[i] = 'z';
        }
        std::vector<OPR> op(N);
        RandInt rand_p(1, N);
        for (auto &[op, x, y] : op) {
            op = rand_op();
            if (op == 0) {
                x = rand_p(), y = rand_lz();
            } else {
                x = 1, y = N;
            }
        }
        output(s, op);
    }

    // random
    for (int i = 0; i < 3; i++) {
        random_free(N, N, rand_c);
        random_free(N, N, rand_lz);
        random_free(N, N, rand_ablz);
    }

    return 0;
}
```