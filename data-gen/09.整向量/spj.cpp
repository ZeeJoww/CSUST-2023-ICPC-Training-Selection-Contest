#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;

const static int AC = 0;
const static int WA = 1;
const static double eps0 = 1e-6;

#define wawa() judgeCode = WA
char tmp[100007];

#define judgeLine(s) \
{ \
    fscanf(pout, "%[^\n]s", tmp); \
    fscanf(pout, "%*[\n]"); \
    if (0 != strcmp(tmp, s)) wawa();\
    /* cout << "expected: " << s << ", read: " << tmp << '\n', wawa(); */\
}

bool solve(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3) {
    if (x1 * y2 != x2 * y1) {
        int64_t au = x2*y3 - x3*y2, ad = x2*y1 - x1*y2;
        int64_t bu = x1*y3 - x3*y1, bd = x1*y2 - x2*y1;
        if (au % ad != 0 || bu % bd != 0) return false;
    } else {
        if (x1 * y3 != x3 * y1) return false;
        if (x1 == 0) {
            std::swap(x1, y1);
            std::swap(x2, y2);
            std::swap(x3, y3);
        }
        int64_t g = std::__gcd(x1, x2);
        if (x3 % g != 0) return false;
    }
    return true;
}

int main (int argc, char* argv[]) {
    /*
     * argv[1]：输入
     * argv[2]：选手输出
     * exit code：返回判断结果
     */
    FILE* fin;
    FILE* pout;
    fin = fopen (argv[1], "r");
    pout = fopen (argv[2], "r");

    int judgeCode = AC;

    int t;
    fscanf(fin, "%d", &t);

    const long long lim = 2e12;

    while (t--) {
        long long x1, y1, x2, y2, x3, y3;
        fscanf(fin, "%Ld %Ld %Ld %Ld %Ld %Ld", &x1, &y1, &x2, &y2, &x3, &y3);

        bool ok = solve(x1, y1, x2, y2, x3, y3);
        if (ok) {
            judgeLine("YES");
            long long a, b;
            fscanf(pout, "%Ld %Ld", &a, &b);
            fscanf(pout, "%*[\n]");
            if (a > lim || a < -lim || b > lim || b < -lim) {
                judgeCode = WA;
            } else if (a * x1 + b * x2 != x3 || a * y1 + b * y2 != y3) {
                judgeCode = WA;
            }
        } else {
            judgeLine("NO");
        }
        if (judgeCode == WA) break;
        // fgetc(pout);
    } // while

    char ch;
    while (EOF != fscanf(pout, "%c", &ch)) if (ch != ' ' && ch != '\n') wawa();

    return judgeCode;
}
