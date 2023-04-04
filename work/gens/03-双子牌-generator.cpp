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
