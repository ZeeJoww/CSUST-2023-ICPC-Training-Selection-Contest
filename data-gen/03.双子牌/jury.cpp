#include <iostream>

constexpr int N = 1e6 + 10, P = 1e9 + 7;
int a[N], b[N], dp[N][2];

int solve() {
    int n;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cin >> a[i] >> b[i];
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

    while (T--) {
        std::cout << solve() << '\n';
    }
    
    return 0;
}
