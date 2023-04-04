#include <tuple>
#include <numeric>
#include <iostream>
#include <algorithm>

void exgcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    int64_t x1 = 1, x2 = 0, x3 = 0, x4 = 1;
    while (b != 0) {
        int64_t c = a / b;
        std::tie(x1, x2, x3, x4, a, b) =
            std::make_tuple(x3, x4, x1 - x3 * c, x2 - x4 * c, b, a - b * c);
    }
    x = x1, y = x2;
}

bool solve() {
    int64_t x1, y1, x2, y2, x3, y3;
    std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    int64_t a, b;
    if (x1 * y2 != x2 * y1) {
        int64_t au = x2*y3 - x3*y2, ad = x2*y1 - x1*y2;
        int64_t bu = x1*y3 - x3*y1, bd = x1*y2 - x2*y1;
        if (au % ad != 0 || bu % bd != 0) return false;
        a = au / ad, b = bu / bd;
    } else {
        if (x1 * y3 != x3 * y1) return false;
        if (x1 == 0) {
            std::swap(x1, y1);
            std::swap(x2, y2);
            std::swap(x3, y3);
        }
        int64_t g = std::__gcd(x1, x2);
        if (x3 % g != 0) return false;
        exgcd(x1, x2, a, b);
        a *= x3 / g;
        b *= x3 / g;
    }

    std::cout << "YES\n";
    std::cout << a << ' ' << b << '\n';

    return true;
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int t;
    std::cin >> t;

    while (t--) {
        if (false == solve()) std::cout << "NO\n";
    }
    
    return 0;
}

