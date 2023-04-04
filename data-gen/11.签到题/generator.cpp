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
