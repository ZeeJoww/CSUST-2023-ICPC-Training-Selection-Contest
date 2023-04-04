#include <iostream>
#include <cassert>

constexpr int N = 1e5 + 10, T = 20;

int Log2[N], st[N][T];

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n, q;
    std::cin >> n >> q;
    assert(1 <= n && n <= 100000);
    assert(1 <= q && q <= 100000);

    Log2[0] = -1;
    for (int i = 1; i <= n; i++) {
        std::cin >> st[i][0];
        assert(1 <= st[i][0] && st[i][0] <= 1000000000);
        Log2[i] = Log2[i/2] + 1;
    }

    for (int t = 0; t < T - 1; t++) {
        for (int i = 1; i + (1<<t) <= n; i++) {
            st[i][t+1] = std::max(st[i][t], st[i+(1<<t)][t]);
        }
    }

    while (q--) {
        int p, k;
        std::cin >> p >> k;
        assert(1 <= p && p <= n && 0 <= k && k <= 1000000000);
        int l = 0, r = p-1;
        while (l < r) {
            int mid = (l + r + 1) / 2;
            int t = Log2[p-mid];
            int val = std::max(st[mid][t], st[p-(1<<t)][t]);
            if (val > st[p][0] + k) l = mid;
            else r = mid - 1;
        }
        if (l == 0) r = -1;
        std::cout << r << '\n';
    }
    
    return 0;
}
