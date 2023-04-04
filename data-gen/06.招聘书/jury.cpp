#include <algorithm>
#include <iostream>
#include <numeric>

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n, m, k;
    std::cin >> n >> m >> k;

    int d[n], f[n];
    int64_t t[n];
    for (int &x : d) std::cin >> x;
    for (int &x : f) std::cin >> x;
    for (int64_t &x : t) std::cin >> x;

    int idx[n];
    std::iota(idx, idx + n, 0);
    auto cmp = [&d] (int x, int y) -> bool {
        return d[x] < d[y];
    };
    std::sort(idx, idx + n, cmp);

    int64_t dp[m+1];
    constexpr int64_t inf = 1e18;
    std::fill(dp, dp + m + 1, inf);
    dp[0] = 0LL;

    for (int i : idx) {
        int bot = std::max(0, d[i] - k);
        for (int x = m - 1; x >= bot; x--) {
            int u = std::min(m, x + f[i]);
            dp[u] = std::min(dp[u], dp[x] + t[i]);
        }
    }

    if (dp[m] == inf) dp[m] = -1;
    std::cout << dp[m] << '\n';
    
    return 0;
}
