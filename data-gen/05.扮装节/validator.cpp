#include <iostream>
#include <cassert>

const int N = 618;
int dis[N][N];

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    int n, m, q;
    std::cin >> n >> m >> q;
    assert(1 <= n && n <= 600);
    assert(1 <= m && m <= 1000);
    assert(1 <= q && q <= 100000);
  
    constexpr int inf = 0x3f3f3f3f;
    for (int i = 1; i <= n; i++) {
        std::fill(dis[i] + 1, dis[i] + n+1, inf);
        dis[i][i] = 0;
    }

    while (m--) {
        int u, v, w;
        std::cin >> u >> v >> w;
        assert(1 <= u && u <= n);
        assert(1 <= v && v <= n);
        assert(1 <= w && w <= 1000000);
        dis[u][v] = std::min(dis[u][v], w);
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                dis[i][j] = std::min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }

    while (q--) {
        int x, y;
        std::cin >> x >> y;
        assert(1 <= x && y <= n);
        assert(1 <= x && y <= n);
        int val = dis[x][y];
        if (val == inf) val = -1;
        std::cout << val << '\n';
    }
    
    return 0;
}
