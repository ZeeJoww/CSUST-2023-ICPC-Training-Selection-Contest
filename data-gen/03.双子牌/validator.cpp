#include <iostream>
#include <cassert>

constexpr int N = 1e6 + 10, P = 1e9 + 7;
int a[N], b[N], dp[N][2];

int sum_n;
int solve() {
    int n;
    std::cin >> n;
    sum_n += n;
    assert(1 <= n && n <= 1000000);
    for (int i = 1; i <= n; i++) {
        std::cin >> a[i] >> b[i];
      assert(1 <= a[i] && a[i] <= 1000000000);
      assert(1 <= b[i] && b[i] <= 1000000000);
    }
    for (int i = 1; i <= n; i++) {
        if (a[i] == b[i]) {
            return 0;
        }
    }

    dp[1][0] = dp[1][1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i][0] = dp[i][1] = 0;
        if (a[i] != a[i-1]) dp[i][0] = (dp[i][0] + dp[i-1][1]) % P;
        if (a[i] != b[i-1]) dp[i][0] = (dp[i][0] + dp[i-1][0]) % P;
        if (b[i] != a[i-1]) dp[i][1] = (dp[i][1] + dp[i-1][1]) % P;
        if (b[i] != b[i-1]) dp[i][1] = (dp[i][1] + dp[i-1][0]) % P;
    }

    return (dp[n][0] + dp[n][1]) % P;
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int T;
    std::cin >> T;
    assert(1 <= T && T <= 10000);

    while (T--) {
        std::cout << solve() << '\n';
    }
    assert(1 <= sum_n && sum_n <= 1000000);
    
    return 0;
}
