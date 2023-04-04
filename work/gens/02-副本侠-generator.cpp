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
