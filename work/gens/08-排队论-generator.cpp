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
