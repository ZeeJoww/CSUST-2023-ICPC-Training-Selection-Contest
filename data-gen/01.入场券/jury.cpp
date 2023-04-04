#include <iostream>
#include <vector>

const int N = 1e7 + 10, P = 998244353;
int cnt[N];

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n;
    std::cin >> n;

    int mx = 0;
    while (n--) {
        int x;
        std::cin >> x;
        for (int i = 2; i * i <= x; i++) {
            while (x % i == 0) {
                cnt[i] ++;
                x /= i;
            }
        }
        cnt[x] ++;
        mx = std::max(mx, x);
    }

    int ans = 1;
    for (int i = 2; i < N; i++) {
        ans = (int64_t) ans * (1 + cnt[i]) % P;
    }
    std::cout << ans << '\n';
    
    return 0;
}
