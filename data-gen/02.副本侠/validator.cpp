#include <cassert>
#include <iostream>

constexpr int P = 998244353, inv100 = 828542813;

int qpow(int a, int b) {
    int res = 1;
    for (; b; b /= 2, a = 1LL * a * a % P) {
        if (b & 1) {
            res = 1LL * res * a % P;
        }
    }
    return res;
}
int inv(int a) {
    return qpow(a, P - 2);
}

int n_sum;

int solve() {
    int n, p1, p2;
    std::cin >> n >> p1 >> p2;
    n_sum += n;
    assert(1 <= n && n <= 1000000);
    assert(0 <= p1 && p1 <= 100);
    assert(0 <= p2 && p2 <= 100);
    assert(p1 + p2 > 0);
    if (p1 < p2) std::swap(p1, p2);

    p1 = 1LL * p1 * inv100 % P;
    p2 = 1LL * p2 * inv100 % P;
    int q1 = (1 - p1 + P) % P;
    int q2 = (1 - p2 + P) % P;
    int qq = 1LL * q1 * q2 % P;
    int inv_1_qq = inv((1 - qq + P) % P); // 1 / (1 - qq)

    int qqc = (qq == 0 ? 1 : inv_1_qq); // (1 + qq + qq^2 + ...) => 1 / (1 - qq)
    int qqo = (qq == 0 ? 1 : 
            (inv_1_qq + 2LL * qq * inv_1_qq % P * inv_1_qq) % P); // (1 + 3*qq + 5*qq^2 + ...)
    int qqe = (qq == 0 ? 2 : (inv_1_qq + qqo) % P);               // (2 + 4*qq + 6*qq^2 + ...)

    int P12 = 1LL * p1 * qqc % P;          //    p1*qqc =    p1*(1 + qq + qq^2 + ...)
    int P11 = 1LL * q1 * p2 % P * qqc % P; // q1*p2*qqc =    p2*(1 + qq + qq^2 + ...)
    int P21 = 1LL * p2 * qqc % P;          //    p2*qqc
    int P22 = 1LL * q2 * p1 % P * qqc % P; // q2*p1*qqc

    int E12 = 1LL * p1 * qqo % P;          //    p1*qqo =    p1*(1 + 3*qq + 5*qq^2 + ...)
    int E11 = 1LL * q1 * p2 % P * qqe % P; // q1*p2*qqe = q1*q2*(2 + 4*qq + 6*qq^2 + ...)
    int E1 = (E11 + E12) % P;
    int E21 = 1LL * p2 * qqo % P;          //    p2*qqo
    int E22 = 1LL * q2 * p1 % P * qqe % P; // q2*p1*qqe
    int E2 = (E21 + E22) % P;

    int ans = 0, dp1 = 1, dp2 = 0;
    for (int i = n; i >= 1; i--) {
        int next_dp1 = (1LL * dp2 * P21 + 1LL * dp1 * P11) % P;
        int next_dp2 = (1LL * dp1 * P12 + 1LL * dp2 * P22) % P;
        int res = (1LL * dp1 * E1 + 1LL * dp2 * E2) % P * i % P;
        dp1 = next_dp1, dp2 = next_dp2;
        ans = (ans + res) % P;
    }
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
    assert(n_sum <= 1000000); 
    
    return 0;
}
