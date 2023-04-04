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
