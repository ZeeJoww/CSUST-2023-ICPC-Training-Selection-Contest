#include <iostream>
#include <cassert>

bool solve() {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
  
    assert(0 <= x1 && x1 <= 1000000000);
    assert(0 <= y1 && y1 <= 1000000000);
    assert(0 <= x2 && x2 <= 1000000000);
    assert(0 <= y2 && y2 <= 1000000000);
    assert(x1 != x2 || y1 != y2);
  
    int dx = x2 - x1, dy = y2 - y1;
    return !(dx > 0 && dx == dy);
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int T;
    std::cin >> T;
    assert(1 <= T && T <= 10000);

    while (T--) {
        std::cout << (solve() ? "YUUMI" : "GG") << '\n';
    }
    
    return 0;
}
