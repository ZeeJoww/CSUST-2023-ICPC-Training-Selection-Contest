#include <iostream>
#include <cassert>

constexpr int N = 1e5, T = 31;

int jmp[N][T];

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n, q;
    std::cin >> n >> q;
  	assert(1 <= n && n <= 100000);
  	assert(1 <= q && q <= 100000);
    
    for (int i = 1; i <= n; i++) {
        std::cin >> jmp[i][0];
      	assert(1 <= jmp[i][0] && jmp[i][0] <= n);
    }
    for (int j = 1; j < T; j++) {
        for (int i = 1; i <= n; i++) {
            jmp[i][j] = jmp[jmp[i][j-1]][j-1];
        }
    }

    while (q--) {
        int p, k;
        std::cin >> p >> k;
      	assert(1 <= p && p <= n);
      	assert(1 <= k && k <= 1000000000);
        for (int j = 0; j < T; j++) {
            if ((k >> j) & 1) p = jmp[p][j];
        }
        std::cout << p << '\n';
    }
    
    return 0;
}
