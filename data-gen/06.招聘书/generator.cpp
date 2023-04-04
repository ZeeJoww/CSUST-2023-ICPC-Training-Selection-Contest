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
        int n = 3, m = 2, k = 1;
        std::vector<int> df { 3,2,1 };
        std::vector<int64> t { 3LL,2LL,1LL };
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
    for (int i = 1; i <= 8; i++) {
        system(("cp ./special-" + std::to_string(i) + ".in " + getPath()).c_str());
    }

    return 0;
}
