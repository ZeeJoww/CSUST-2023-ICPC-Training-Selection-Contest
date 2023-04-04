#include <iostream>

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int64_t n, sum = 0, mn = 1e9 + 7;
    std::cin >> n;
    while (n--) {
        int x;
        std::cin >> x;
        sum += x;
        mn = std::min(mn, (int64_t) x);
    }
    sum -= 2 * mn;
    std::cout << sum << '\n';
    
    return 0;
}
