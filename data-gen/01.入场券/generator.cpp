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
    hand_include(maxn, primes.begin() + (primes.end() - primes.begin()) / 2);

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
