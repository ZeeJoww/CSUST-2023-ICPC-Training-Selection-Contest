#include <iostream>

constexpr int N = 2e5 + 10;

struct Node {
    int64_t l, z, lz, zl, lzl;
    Node() : l(0), z(0), lz(0), zl(0), lzl(0) {}
    Node(char c) : Node() {
        if (c == 'l') l = 1;
        if (c == 'z') z = 1;
    }
} tr[N * 4];

Node operator + (const Node &L, const Node &R) {
    Node M;
    M.l = L.l + R.l;
    M.z = L.z + R.z;
    M.lz = L.lz + L.l * R.z + R.lz;
    M.zl = L.zl + L.z * R.l + R.zl;
    M.lzl = L.lzl + L.lz * R.l + L.l * R.zl + R.lzl;
    return M;
}

int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    
    std::string s;
    std::cin >> s;
    s = '.' + s;

    int n = s.size(), M;
    for (M = 1; M + 2 <= n; M *= 2) {}
    for (int i = 1; i <= n; i++) {
        tr[M + i] = Node(s[i]);
    }
    for (int i = M - 1; i; i--) {
        tr[i] = tr[i*2] + tr[i*2+1];
    }

    int m;
    std::cin >> m;

    while (m--) {
        std::string opr;
        std::cin >> opr;
        if (opr == "change") {
            int p; char c;
            std::cin >> p >> c;
            p += M;
            tr[p] = Node(c);
            for (int s=p/2; s; s/=2) {
                tr[s] = tr[s*2] + tr[s*2+1];
            }
        } else /* (opr == "count-lzl") */ {
            int l, r;
            std::cin >> l >> r;
            Node res_l, res_r;
            for (int s=l+M-1, t=r+M+1; s^t^1; s/=2, t/=2) {
                if (~s&1) res_l = res_l + tr[s^1];
                if ( t&1) res_r = tr[t^1] + res_r;
            }
            Node res = res_l + res_r;
            std::cout << res.lzl << '\n';
        }
    }
    
    return 0;
}
