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
