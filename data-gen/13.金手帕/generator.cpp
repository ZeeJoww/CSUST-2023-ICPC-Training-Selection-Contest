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
        a[map[i+1]-1] = map[a0[i]];
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
