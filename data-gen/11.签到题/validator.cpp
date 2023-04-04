#include <iostream>
#include <cassert>

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int64_t n, sum = 0, mn = 1e9 + 7;
    std::cin >> n;
    assert(1 <= n && n <= 500000);
  
    while (n--) {
        int x;
        std::cin >> x;
        assert(1 <= x && x <= 1000000000);
        sum += x;
        mn = std::min(mn, (int64_t) x);
    }
    sum -= 2 * mn;
    std::cout << sum << '\n';
    
    return 0;
}
