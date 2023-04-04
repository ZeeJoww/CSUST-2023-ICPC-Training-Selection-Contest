#include <iostream>
#include <vector>

const int N = 1e5 + 10;

std::vector<int> edge[N];
int a[N], tin[N], tout[N], tot;

void dfs(int u, int p) {
    tin[u] = ++tot;
    for (int v : edge[u]) {
        if (v == p) continue;
        dfs(v, u);
    }
    tout[u] = tot;
}

int64_t tr[N];

void add(int s, int val) {
    for (int x = s; x <= tot; x += x & -x) {
        tr[x] += val;
    }
}

int64_t query(int s) {
    int64_t res = 0LL;
    for (int x = s; x; x -= x & -x) {
        res += tr[x];
    }
    return res;
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n, q;
    std::cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        std::cin >> a[i];
    }
    for (int i = 2; i <= n; i++) {
        int u, v;
        std::cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }

    dfs(1, 0);
    for (int i = 1; i <= n; i++) {
        add(tin[i], a[i]);
        add(tin[i] + 1, -a[i]);
    }

    while (q--) {
        int op, p;
        std::cin >> op >> p;
        if (op == 1) {
            int d;
            std::cin >> d;
            add(tin[p], -d);
            add(tout[p] + 1, +d);
        } else {
            std::cout << query(tin[p]) << '\n';
        }
    }
    
    return 0;
}
