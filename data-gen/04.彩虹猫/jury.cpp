#include <iostream>

bool solve() {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    int dx = x2 - x1, dy = y2 - y1;
    return !(dx > 0 && dx == dy);
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int T;
    std::cin >> T;

    while (T--) {
        std::cout << (solve() ? "YUUMI" : "GG") << '\n';
    }
    
    return 0;
}
