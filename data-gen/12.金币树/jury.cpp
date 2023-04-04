#include <iostream>
#include <vector>

constexpr int N = 5e5 + 10;

int64_t a[N];
std::vector<int> edge[N];

int64_t ans;
void dfs(int u, int p) {
    int64_t mx = 0;
    for (int v : edge[u]) {
        if (v == p) continue;
        dfs(v, u);
        ans -= a[v];
        mx = std::max(a[v], mx);
    }
    a[u] += mx;
    ans += mx * (edge[u].size() - (p != -1));
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);

    int n;
    std::cin >> n;

    for (int i = 1; i <= n; i++) {
        std::cin >> a[i];
    }
    for (int i = 2; i <= n; i++) {
        int u, v;
        std::cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    dfs(1, -1);
    std::cout << ans << '\n';
    
    return 0;
}
