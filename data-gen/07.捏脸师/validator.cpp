#include <iostream>
#include <cassert>
#include <vector>
#include <stack>

constexpr int N = 5e5 + 10, inf = 0x3f3f3f3f;
std::vector<int> edge[N];
int p[N];

int low[N], dfn[N], scc[N], scc_weight[N], indegree[N];
int now, scc_cnt;
bool instack[N];
std::stack<int> sta;

void dfs(int u) {
    low[u] = dfn[u] = ++now;
    sta.push(u);
    instack[u] = true;
    for (int v : edge[u]) {
        if (dfn[v] == 0) {
            dfs(v);
            low[u] = std::min(low[u], low[v]);
        } else if (instack[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }

    if (low[u] == dfn[u]) {
        int x;
        int num = ++ scc_cnt;
        scc_weight[num] = inf;
        do {
            x = sta.top();
            sta.pop();
            instack[x] = false;
            scc[x] = num;
            scc_weight[num] = std::min(scc_weight[num], p[x]);
            for (int v : edge[x]) {
                if (scc[v] == num) continue;
                indegree[scc[v]] ++;
            }
        } while (x != u);
    }
}

int sum_n, sum_m;

int64_t solve() {
    int n, m;
    std::cin >> n >> m;
    sum_n += n, sum_m += m;
    assert(2 <= n && n <= 500000);
    assert(1 <= m && m <= 500000);

    for (int i = 1; i <= n; i++) {
        std::cin >> p[i];
        assert(1 <= p[i] && p[i] <= 1000000000);
    }
    while (m--) {
        int u, v;
        std::cin >> u >> v;
        assert(1 <= u && u <= n && 1 <= v && v <= n && u != v);
        edge[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            dfs(i);
        }
    }

    int64_t ans = 0;
    for (int i = 1; i <= scc_cnt; i++) {
        if (indegree[i] == 0) {
            ans += scc_weight[i];
        }
    }
    for (int i = 1; i <= n; i++) {
        edge[i].clear();
        indegree[i] = low[i] = dfn[i] = scc[i] = 0;
    }
    scc_cnt = now = 0;
    return ans;
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);

    int T;
    std::cin >> T;
    assert(1 <= T && T <= 10000);

    while (T--) {
        std::cout << solve() << '\n';
    }
    assert(1 <= sum_n && sum_n <= 500000);
    assert(1 <= sum_m && sum_m <= 500000);
    
    return 0;
}
