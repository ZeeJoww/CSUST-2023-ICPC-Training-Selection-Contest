# 干饭协会的入场券

## 题目描述

> This problem is the ticket of the Eat-More Union.

You are given an array of $n$ positive integers $a_1,a_2,\cdots,a_n$.

Please count the number of factors of $\displaystyle \prod_{i=1}^{n} a_i$.

## 输入格式

The first line contains a single integer $n$ ( $1 \leqslant n \leqslant {10}^{5}$ ).

The second line contains $n$ integers $a_1,a_2,\cdots,a_n$ ( $1 \leqslant a_i \leqslant {10}^{7}$ ).

## 输出格式

Print a single integer — the number of factors of $\displaystyle \prod_{i=1}^{n} a_i$. Since the answer can be very large, so print it modulo $998244353$.

## 样例

输入：

```
5
1 2 3 4 5
```

输出：

```
16
```

## 提示

$$
\displaystyle \prod_{i=1}^{n} a_i = a_1 \cdot a_2 \cdot a_3 \cdots a_n
$$

```cpp
//=======================
//01-入场券-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 1e7;

std::vector<int> primes;
bool notPrime[N+1];

void init0() {
    for (int i = 2; i <= N; i++) {
        if (notPrime[i]) continue;
        primes.push_back(i);
        for (int j = i*2; j <= N; j += i) {
            notPrime[j] = true;
        }
    }
}

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
    init0();
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

class NoDupRandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
    std::set<int> except;
public:
    NoDupRandInt(int lb, int ub) : u(lb, ub) {}
    NoDupRandInt(int lb, int ub, const std::set<int> &set) : u(lb, ub), except(set) {}
    int operator() () {
        int res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

void output(const std::vector<int> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const int &x : vec) {
        fout << x << " \n"[&x == &*vec.rbegin()];
    }
    fout.close();
}

void hand_include(int n, auto iter) {
    std::vector<int> hand(iter, iter + n);
    output(hand);
}

void random(int n, auto randInt) {
    std::vector<int> res(n);
    for (int &x : res) {
        x = randInt();
    }
    output(res);
}

void random_free(int n, int lb, int ub) {
    random(n, NoDupRandInt(lb, ub));
}

void random_except(int n, int lb, int ub, const std::vector<int> &e0) {
    random(n, NoDupRandInt(lb, ub, std::set<int>(e0.begin(), e0.end())));
}

void random_from(int n, const std::vector<int> &from) {
    NoDupRandInt randInt(0, from.size() - 1);
    std::vector<int> res(n);
    for (int &x : res) {
        x = from[randInt()];
    }
    output(res);
}

int main() {
    init();

    constexpr int maxn = 1e5;

    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    hand_include(5, vec.begin());

    hand_include(maxn, primes.begin());
    hand_include(maxn, primes.rbegin());
    hand_include(maxn, primes.begin() + (primes.end() - primes.begin()));

    /* tmp */ {
        std::vector<int> tmp;
        for (int i = N, c = 0; i >= 0; i--) {
            if (notPrime[i]) {
                tmp.push_back(i);
                c++;
                if (c == maxn) {
                    break;
                }
            }
        }
        hand_include(maxn, tmp.begin());
    }

    for (int i = 0; i < 5; i++) {
        random_free(maxn, 1, N);
    }
    for (int i = 0; i < 5; i++) {
        random_except(maxn, 1, N, primes);
    }
    for (int i = 0; i < 5; i++) {
        random_from(maxn, primes);
    }

    for (int i = 1; i <= 5; i++) {
        std::vector<int> vec(maxn);
        RandInt randInt(N - (1 << i), N);
        for (int &x : vec) {
            x = randInt();
        }
        output(vec);
    }

    for (int i = 1; i <= 5; i++) {
        std::vector<int> vec(maxn);
        RandInt randInt(primes.size() - (1 << i), primes.size());
        for (int &x : vec) {
            x = primes[randInt() - 1];
        }
        output(vec);
    }

    return 0;
}
```

# 干饭协会的副本侠

## Description

阿水是 $\text{EatMore}$ 干饭协会里著名的副本侠，因为阿水可以很轻松的通关各式各样的副本。

今天游戏里出了一个新副本《云梦泽》，副本侠阿水早已跃跃欲试了。

<center>
<img src="resource/map.jpg" width="320px">
<br>
  <div style="color:gray">
    新副本《云梦泽》地图
  </div>
</center>

《云梦泽》里共有 $n$ 只怪兽，由于 $3202$ 年的怪兽十分强大，只有使用技能才有机会杀死怪兽。

阿水有两个技能，每个技能需要花费 $1$ 秒的时间来释放，然后该技能会进入为期 $1$ 秒的冷却时间（冷却时不能释放该技能，但可以释放另一个技能）。

每次释放技能可以指定一只怪兽，且阿水的两个技能分别有 $p_1\%, p_2\%$ 的概率杀死怪兽。

阿水会以 $1$ 秒每次的频率轮流释放两个技能，直至所有怪兽都被杀死（即通关该副本）；在每次释放技能的 过程中，阿水会受到所有存活怪兽的单次攻击，**即每只怪兽被杀死前会对阿水造成一次伤害。**

阿水听说你擅长计算，于是问你：在最优情况下，他通关副本所受到怪兽攻击次数的期望是多少？

## Input

本题包含多组测试数据。

第一行有一个整数 $T$ ( $1 \leqslant T \leqslant 10000$ )，表示一共有 $T$ 组测试数据。

接下来每一行有三个整数，表示新副本中的怪兽数量 $n$ ( $1 \leqslant n \leqslant 5 \cdot {10}^{6}$ ) ，以及阿水两个技能击杀怪兽概率的 $100$ 倍 $p_1, p_2$ ( $0 \leqslant p_1, p_2 \leqslant 100,~ p_1 + p_2 > 0$ )。

保证所有测试数据的怪兽数量之和 $\displaystyle \sum n $ 不超过 $ {10}^{6}$。

## Output

输出共 $T$ 行，对于每组测试数据输出一行一个非负整数，表示最优情况下，阿水受到怪兽攻击次数的期望对 $998244353$ 取模后的结果。

可以证明，所求期望一定是一个有理数，设其为 $\displaystyle \frac{p}{q} (gcd(p, q) = 1)$，那么你输出的非负整数 $x$ 要满足 $p \equiv qx \pmod{998244353}$。

## Sample #1

### Sample Input #1

```
2
1 50 50
5 0 100
```

### Sample Output #1

```
2
25
```

## Hints

对于第一组测试数据，仅有一只怪兽，且阿水每秒都有 $\displaystyle \frac{1}{2}$ 的概率杀死怪兽。
因此，阿水会受到怪兽的第 $1$ 次攻击，有 $\displaystyle \frac{1}{2}$ 的概率受到第 $2$ 次攻击，$\cdots$，有 $\displaystyle \frac{1}{2^{n-1}}$ 的概率受到第 $n$ 次攻击，$\cdots$
所以，阿水受到怪兽攻击次数的期望为 $\displaystyle \sum_{i=0}^{\infty} \displaystyle \frac{1}{2^i} = 2$。

对于第二组测试数据，由于技能 $1$ 不可能杀死怪兽，而技能 $2$ 一定能杀死怪兽，因此阿水在释放完 $5$  次技能 $2$ 后就能杀死所有怪兽。
阿水可以首先释放技能 $2$，即在第 $1,3,5,7,9$ 秒初释放技能 $2$，在第 $2,4,6,8$ 秒初释放技能 $1$；于是在 $9$ 秒末就能杀死所有怪兽，$9$ 秒内各秒分别受到 $5,4,4,3,3,2,2,1,1$ 次怪兽的攻击，共 $25$ 次攻击。

```cpp
//=======================
//02-副本侠-generator.cpp
#include <fstream>
#include <random>
#include <vector>

constexpr int N = 1e6, topcnt = 10000;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

RandInt getp(0, 100);

struct Testcase {
    int n, p1, p2;
    Testcase(int _n, int _p1, int _p2) : n(_n), p1(_p1), p2(_p2) {}
    Testcase(int _n) : n(_n) {
        do {
            p1 = getp(), p2 = getp();
        } while (p1 + p2 == 0);
    }
};

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &[n, p1, p2] : vec) {
        fout << n << ' ' << p1 << ' ' << p2 << '\n';
    }
    fout.close();
}

void random_free(int lb, int ub) {
    RandInt getn(lb, ub);
    int sum_n = 0, cnt = 0;
    std::vector<Testcase> res;
    while (true) {
        int n = getn();
        sum_n += n;
        cnt ++;
        if (cnt > topcnt || sum_n > N) break;
        res.emplace_back(n);
    }
    output(res);
}

int main() {
    init();

    { // hand cases: sample
        std::vector<Testcase> tmp;
        tmp.emplace_back(1, 50, 50);
        tmp.emplace_back(5, 0, 100);
        output(tmp);
    }

    { // hand cases: all cases with a zero
        std::vector<Testcase> tmp;
        for (int i = 1; i <= 100; i++) {
            tmp.emplace_back(5000, i, 0);
            tmp.emplace_back(5000, 0, i);
        }
        output(tmp);
    }

    { // hand cases: all cases without any zero
        std::vector<Testcase> tmp;
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                tmp.emplace_back(10, i, j);
            }
        }
        output(tmp);
    }

    // random cases: n is tiny
    for (int i = 0; i < 3; i++) {
        random_free(1, 50);
    }

    // random cases: n is normal
    for (int i = 0; i < 3; i++) {
        random_free(50, 1000);
    }

    // random cases: n is large
    for (int i = 0; i < 3; i++) {
        random_free(1000, 10000);
    }

    // random cases: n == N
    for (int i = 0; i < 3; i++) {
        random_free(N, N);
    }

    return 0;
}
```

# 干饭协会的双子牌

## 题目描述

$\text{EatMore}$ 干饭协会发明了一款牌类游戏，会员们都管它叫双子牌，因为每张牌都有左右两个数字，如 下图是一张 ($2,7$) 牌（两个数字分别是 $2$ 和 $7$）。

<center>
<img src="resource/single-card.png" width="250px">
<br>
  <div style="color:gray">
    (2,7) 双子牌
  </div>
</center>

当然，你可以将这张牌旋转 $180\degree$，使其变成一张 ($7,2$) 牌，如下图所示。

<center>
<img src="resource/two-cards.png" width="520px">
<br>
  <div style="color:gray">
    (2,7) 牌 == (7,2) 牌
  </div>
</center>

现有 $n$ 张双子牌排成一行，其中从左数起的第 $i$ 张牌是 ($a_i, b_i$) 牌。

你可以进行任意多次如下操作：
- 从 $n$ 张牌里选择一张牌 ($a_i, b_i$) 牌，将其旋转 $180\degree$ 变成 ($b_i, a_i$) 牌。

在完成操作之后，把这一行双子牌变成一个长度为 $2n$ 的数组 $c = [c_1, c_2, c_3, c_4, \cdots, c_{2n}] = [a_1, b_1, a_2, b_2, \cdots, a_n, b_n]$。

请问你能得到多少种**相邻元素不相等**的数组（即对于任意的 $1 \leqslant i < 2n$，满足 $c_i \neq c_{i+1}$）？可以证明，满足要求的不同数组数量有限。由于答案可能很大，输出其对 ${10}^9 + 7$ 取模后的结果。

作为提示，两个数组相等当且仅当两个数组长度相等且相同下标的元素值相等。

## 输入格式

本题包含多组测试数据。

第一行有一个整数 $T$ ( $1 \leqslant T \leqslant 10000$ )，表示一共有 $T$ 组测试数据。

对于每组测试数据：

- 第一行有 $1$ 个整数 $n$ ( $1 \leqslant n \leqslant {10}^{6}$ )，表示共有 $n$ 张双子牌。
- 接下来 $n$ 行中，第 $i$ 行有 $2$ 个整数 $a_i, b_i$ ( $1 \leqslant a_i, b_i \leqslant {10^9}, 1 \leqslant i \leqslant n$ )，表示初始时刻第 $i$ 张双子牌为 ($a_i, b_i$) 牌。

保证将所有测试数据的双子牌全部叠在一起后，总张数 $\displaystyle \sum n$ 不超过 ${10}^6$。

## 输出格式

输出共 $T$ 行，对于每组测试数据输出一行一个整数表示答案（对 ${10}^9 + 7$ 取模）。

## 样例 #1

### 样例输入 #1

```
2
2
3 5
5 7
2
114514 114514
114514 233333
```

### 样例输出 #1

```
3
0
```

## 提示

对于样例第一组测试数据，下图列出了所有可能的数组，显然答案为 $3$。

<center>
<img src="resource/lots-of-cards.png" width="500px">
<br>
  <div style="color:gray">
    第一组测试数据示意图
  </div>
</center>

```cpp
//=======================
//03-双子牌-generator.cpp
#include <fstream>
#include <random>
#include <vector>

constexpr int N = 1e6, lim = 1e9, topcnt = 10000;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Card {
    int a, b;
};

typedef std::vector<Card> Testcase;

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &cards : vec) {
        fout << cards.size() << '\n';
        for (const auto &[a, b] : cards) {
            fout << a << ' ' << b << '\n';
        }
    }
    fout.close();
}

void random_free(int lb, int ub, int min, int max) {
    RandInt getn(lb, ub), randInt(min, max);
    int sum_n = 0, cnt = 0;
    std::vector<Testcase> res;
    while (true) {
        int n = getn();
        cnt ++;
        if (n + sum_n > N) {
            n = N - sum_n;
        }
        sum_n += n;
        Testcase cards(n);
        for (auto &[a, b] : cards) {
            do {
                a = randInt(), b = randInt();
            } while (a == b);
        }
        res.emplace_back(cards);
        if (cnt == topcnt || sum_n == N) break;
    }
    output(res);
}

int main() {
    init();

    { // hand cases: sample
        std::vector<Testcase> tmp;
        tmp.emplace_back(Testcase{ {3,5}, {5,7} });
        tmp.emplace_back(Testcase{ {114514,114514}, {114514,233333} });
        output(tmp);
    }

    { // hand cases: containing same
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(1, 1000);
        for (auto &vec : tmp) {
            for (auto &[a, b] : vec) {
                do {
                    a = randInt(), b = randInt();
                } while (a == b);
            }
            vec[randInt() - 1] = ({ int a, b; do {
                    a = randInt(), b = randInt();
                } while (a == b); Card{a, b};
            });
        }
        output(tmp);
    }

    { // hand cases: only 1 solution
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(100, 1000);
        for (auto &vec : tmp) {
            int a0, b0;
            do {
                a0 = randInt(), b0 = randInt();
            } while (a0 == b0);
            for (auto &[a, b] : vec) {
                a = a0, b = b0;
                std::swap(a0, b0);
            }
        }
        output(tmp);
    }

    { // hand cases: specified the differing number
        std::vector<Testcase> tmp(1000, Testcase(1000));
        RandInt randInt(1, 10*N-1);
        std::vector<Card> pool;
        for (auto &vec : tmp) {
            pool.push_back( Card {randInt(), randInt()} );
            RandInt choose(0, pool.size() - 1);
            for (auto &card : vec) {
                card = pool[choose()];
            }
        }
        output(tmp);
    }

    // random cases: smaller
    random_free(50, 150, 1, lim);

    // random cases: larger
    random_free(5e4, 1e5, 1, lim);

    // random cases: only one testcase
    random_free(N, N, 1, lim);

    return 0;
}
```

# 干饭协会的彩虹猫

## Description

$\text{EatMore}$ 干饭协会有一只彩虹猫，名叫悠米，悠米从来不用腿走路，因为悠米拥有一本魔典，只要悠 米吃够了协会食堂为她特供的彩虹猫粮，悠米就能站在魔典上释放魔法、自由飞行。

<center>
<img src="https://acm.csust.edu.cn/public/upload/46634a2873.gif" width="320px">
<br>
  <div style="color:gray">
    悠米正在飞行！
  </div>
</center>

有一天，悠米吃完彩虹猫粮后困意袭来，打了个盹却开始做噩梦了，梦见她在一个无限大的二维棋盘上和恶魔进行一场回合制游戏，游戏规则如下：

- 在悠米的回合，悠米可以选择从 $(x, y)$ 移动到 $(x+1,y)$ 或 $(x, y+1)$。
- 在恶魔的回合，恶魔先选择棋盘上一个**已放置**怪兽的格子 $(x, y)$，然后从 $(x+1,y), (x,y+1), (x-1,y), (x,y-1)$ 中**选择一格**再放置一只怪兽，并且**可以放在悠米所在的格子**。
- 如果某一时刻悠米与怪兽位于同一格，则悠米会被怪兽抓住，即逃离失败；否则悠米永远也不会遇到怪兽，即逃离成功。

假设悠米一开始位于 $(x_1, y_1)$，且初始时刻，棋盘上有 $1$ 只怪兽位于 $(x_2, y_2)$。

如下图中浅蓝色区域表示悠米可以移动到的格子，浅红色区域表示恶魔可以放置新的怪兽的格子。

<center>
<img src="resource/悠米 VS 恶魔.png" width="960px">
<br>
  <div style="color:gray">
    悠米 VS 恶魔
  </div>
</center>

第一个回合是悠米的回合，已知悠米和恶魔均会以最优方式进行游戏。

问：悠米能否成功逃离恶魔的围困？

## Input

本题包含多组测试数据。

第一行有一个整数 $T$ ( $1 \leqslant T \leqslant 10000$ )，表示一共有 $T$ 组测试数据。

接下来每行包括 $4$ 个非负整数 $x_1, y_1, x_2, y_2$ ( $0 \leqslant x_1, y_1, x_2, y_2 \leqslant {10}^{9}$ )，表示游戏开始时悠米的坐标 $(x_1,y_1)$ 与初始棋子的坐标 $(x_2, y_2)$，保证两个坐标不相同，即 $(x_1,y_1) \neq (x_2,y_2)$。

注意，数据范围仅表示初始坐标在 $[0, {10}^9]$ 范围内；在游戏中，悠米可以走出该范围，怪兽也可以被放 置在该范围之外，只要满足相应规则即可。

## Output

输出共 $T$ 行，对于每组测试数据输出一行，若悠米能成功逃离恶魔的围困，输出 `YUUMI`，否则输出 `GG`。

## Sample #1

### Sample Input #1

```
3
4 3 9 9
1 2 3 4
3 3 3 2
```

### Sample Output #1

```
YUUMI
GG
YUUMI
```


```cpp
//=======================
//04-彩虹猫-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int T = 10000, lim = 1e9;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Testcase {
    int x1, y1, x2, y2;
};

void output(const std::vector<Testcase> &vec) {
    std::ofstream fout(getPath());
    fout << vec.size() << '\n';
    for (const auto &[x1, y1, x2, y2] : vec) {
        fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';
    }
    fout.close();
}

int main() {
    init();

    { // hand: sample
        std::vector<Testcase> tmp { {4,3,9,9}, {1,2,3,4}, {3,3,3,2} };
        output(tmp);
    }

    { // hand: small & YUUMI
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, 1000);
        std::set<std::pair<int, int>> has;
        for (auto &[a, b, c, d] : tmp) {
            while (true) {
                int x = randInt(), y = randInt();
                if (x == y) continue;
                if (x > y) std::swap(x, y);
                if (has.count(std::pair<int,int>(x,y))) continue;
                has.emplace(x, y);
                a = b = x, c = d = y;
                break;
            }
        }
        output(tmp);
    }

    { // hand: small & GG
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, 1000);
        for (auto &[a, b, c, d] : tmp) {
            do {
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
            } while (c - a == d - b && c - a > 0);
        }
        output(tmp);
    }

    { // hand: larger & YUUMI
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, lim);
        std::set<std::pair<int, int>> has;
        for (auto &[a, b, c, d] : tmp) {
            while (true) {
                int x = randInt(), y = randInt();
                if (x == y) continue;
                if (x > y) std::swap(x, y);
                if (has.count(std::pair<int,int>(x,y))) continue;
                has.emplace(x, y);
                a = b = x, c = d = y;
                break;
            }
        }
        output(tmp);
    }

    { // random
        std::vector<Testcase> tmp(T);
        RandInt randInt(0, lim);
        for (auto &[a, b, c, d] : tmp) {
            do {
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
            } while (a == c && b == d);
        }
        output(tmp);
    }

    return 0;
}
```

# 干饭协会的扮装节

## Description

In the $\text{EatMore}$ Union, there are $n$ characters present at the party to celebrate the *Cosplay Festival*$~$(扮装节).

They enjoy cosplaying others in this party, and if $a$ can cost $x$ to cosplay $b$, and $b$ can cost $y$ to cosplay $c$, then we conclude that $a$ can also cost $x + y$ to cosplay $c$.

For example, let $a$ be *Pikachu*$~$(皮卡丘), $b$ be *Shin-Chan*$~$(蜡笔小新), $c$ be *Naruto*$~$(鸣人), then we conclude cosplaying relations as described in the figure below:

<center>
<img src="resource/cos-rela.png" width="960px">
<br>
  <div style="color:gray">
    cosplaying relations
  </div>
</center>


By the way, one can always cosplay himself/herself without any cost, i.e. $a$ costs $0$ to cosplay $a$.

You are given $m$ cosplaying relations, described as $3$ integers $a, b, w$, meaning that $a$ can cost $w$ to cosplay $b$.

You are also given $q$ querys. In each query, there are $2$ integers $x, y$ — if $x~$**cannot**$~$cosplay $y$, output $-1$; otherwise, output the minimal cost for $x$ to cosplay $y$.

## Input Format

The first line contains $3$ integers $n$, $m$, and $q$ ( $1 \leqslant n \leqslant 600, 1 \leqslant m \leqslant {10}^{3}, 1 \leqslant q \leqslant {10}^{5}$ ) — the number of people in this town, the number of cosplaying relations, and the number of querys.

Then $m$ lines follow, where $i$-th line contains $3$ integers $a_i, b_i, w_i$ ( $ 1 \leqslant a_i, b_i \leqslant n$, $1 \leqslant w_i \leqslant {10}^6$, $1 \leqslant i \leqslant m$ ) , meaning that $a_i$ costs $w_i$ to cosplay $b_i$.

Then $q$ lines follow, where $j$-th line contains two integers $x_i$ and $y_i$ ( $ 1 \leqslant x_j, y_j \leqslant n$, $1 \leqslant j \leqslant q$ ) , representing a query — if $x_j~$**cannot**$~$cosplay $y_j$, output $-1$; otherwise, output the minimal cost for $x_j$ to cosplay $y_j$.

## Output Format

Output $q$ lines, where $j$-th line represents the answer to $j$-th query.

### Sample #1

#### Sample Input #1

```
2 2 1
2 1 3
2 1 6
2 1
```

#### Sample Output #1

```
3
```

### Sample #2

#### Sample Input #2

```
4 5 3
2 4 9
1 3 6
3 4 1
1 2 2
2 3 3
2 1
1 1
1 4
```

#### Sample Output #2

```
-1
0
6
```

```cpp
//=======================
//05-扮装节-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int max_weight = 1e6;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

class RandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
public:
    RandInt(int lb, int ub) : u(lb, ub) {}
    int operator() () {
        return u(e);
    }
};

class NoDupRandInt {
    std::uniform_int_distribution<std::mt19937::result_type> u;
    std::set<int> except;
public:
    NoDupRandInt(int lb, int ub) : u(lb, ub) {}
    NoDupRandInt(int lb, int ub, const std::set<int> &set) : u(lb, ub), except(set) {}
    int operator() () {
        int res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct Edge {
    int u, v, w;
};

struct Query {
    int x, y;
};

void output(int n, int m, int q, auto edges, auto querys) {
    std::ofstream fout(getPath());
    fout << n << ' ' << m << ' ' << q << '\n';
    for (auto [u, v, w] : edges) {
        fout << u << ' ' << v << ' ' << w << '\n';
    }
    for (auto [x, y] : querys) {
        fout << x << ' ' << y << '\n';
    }
    fout.close();
}

std::vector<Edge> getEdges(int m, RandInt &randInt) {
    NoDupRandInt randWeight(1, max_weight);
    std::vector<Edge> edges(m);
    std::set<std::pair<int,int>> has;
    for (auto &[u, v, w] : edges) {
        do {
            u = randInt(), v = randInt(), w = randWeight();
        } while (has.count(std::make_pair(u, v)));
        has.emplace(u, v);
    }
    return edges;
}

std::vector<Query> getQuerys(int q, RandInt &randInt) {
    std::vector<Query> querys(q);
    std::set<std::pair<int,int>> has;
    for (auto &[x, y] : querys) {
        do {
            x = randInt(), y = randInt();
        } while (has.count(std::make_pair(x, y)));
        has.emplace(x, y);
    }
    return querys;
}

void random_free(int n, int m, int q) {
    RandInt randInt(1, n);
    auto edges = getEdges(m, randInt);
    auto querys = getQuerys(q, randInt);
    output(n, m, q, edges, querys);
}

int main() {
    init();

    { // sample
        int n = 2, m = 2, q = 1;
        std::vector<Edge> edges { {2,1,3}, {2,1,6} };
        std::vector<Query> querys { {2,1} };
        output(n, m, q, edges, querys);
    }

    constexpr int N(600), M(1000), Q(100000);

    { // hand
        int n = 501, m = M, q = Q;
        std::vector<Edge> edges;
        RandInt randWeight(1, max_weight);
        for (int i = 2; i <= n; i++) {
            edges.push_back( Edge{1, i, randWeight()} );
            edges.push_back( Edge{i, 1, randWeight()} );
        }
        RandInt randInt(1, n);
        auto querys = getQuerys(q, randInt);
        output(n, m, q, edges, querys);
    }

    // random
    for (int i = 0; i < 8; i++) {
        random_free(N, M, Q);
    }

    return 0;
}
```

# 干饭协会的招聘书

## Description

由于 $\text{EatMore}$ 干饭协会日益壮大，现已无法依靠纯人工处理繁忙的业务。因此，会长 $\text{lzl}$ 决定招聘一批编程高手，借助计算机程序来处理各项业务。

招聘要求：应聘者在协会自建的 $\text{EatMore Online Judge}$网站上（简称 $\text{emoj}$），评分不低于 $m$ 就可以投递简历。

<center>
<img src="resource/emoj.png" width="640px">
<br>
  <div style="color:gray">
    emoj 网站界面
  </div>
</center>

为了更好地帮助大家提升编程水平并合理赋分，$\text{emoj}$ 有如下规定：

- $\text{emoj}$ 上的每道题都有一定的难度 $d$，分值 $f$。
- 当一名用户**第一次**通过一道分值为 $f$ 的题目时，该用户的评分增加 $f$（过题即得分，但**不能重复 获得**同一道题的分值）。
- 如果某题难度 $d$ 与某用户评分 $r$ 之差**超过**阈值 $k$（即 $d - r > k$），则该用户**不能**做这道题目。

$\text{pyq}$ 掌握了如上信息之后，立马就开始筹备他的 “假应聘，真篡位” 计划了。不过，在行动之前，他 必须要能投得进简历才行。

已知 $\text{emoj}$ 共有 $n$ 道题，$\text{pyq}$ 做完第 $i$ 道题需要耗费时间 $t_i$。

现在 $\text{pyq}$ 注册了一个评分为 $0$ 的新的 $\text{emoj}$ 账号，他想知道，至少要花多少时间才能使评分达到 $m$ ，从而拥有投递简历的机会呢？

如果 $\text{pyq}$ 永远也不能使评分到达 $m$，则输出 $-1$。

## Input

第一行有 $3$ 个整数，分别表示 $\text{emoj}$ 的题目总数 $n$ ( $1 \leqslant n \leqslant 3000$ )，投 递简历需要的评分 $m$ ( $1 \leqslant m \leqslant 3000$ )，以及阈值 $k$ ( $1 \leqslant k \leqslant 3000$ )。

第二行有 $n$ 个整数，其中第 $i$ 个整数表示第 $i$ 道题的难度 $d_i$ ( $1 \leqslant d_i \leqslant 3000$ )。

第三行有 $n$ 个整数，其中第 $i$ 个整数表示第 $i$ 道题的分值 $f_i$ ( $1 \leqslant f_i \leqslant 3000$ )。

第四行有 $n$ 个整数，其中第 $i$ 个整数表示 $\text{pyq}$ 做第 $i$ 道题需耗时 $t_i$ ( $1 \leqslant t_i \leqslant {10}^{12}$ )。

## Output

输出一个整数 $T$，表示 $\text{pyq}$ 至少要花费时间 $T$ 才能够拥有投递简历的机会；如果 $\text{pyq}$ 永远也不能使评分到达 $m$，则输出 $-1$。

## Sample #1

### Sample Input #1

```
1 2 3
1 2 3
1 2 3
1 2 3
```

### Sample Output #1

```
2
```

## Sample #2

### Sample Input #2

```
2 1 1
2 9
3 8
9876543210 123467890
```

### Sample Output #2

```
-1
```

```cpp
//=======================
//06-招聘书-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 3000;
using int64 = long long;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

template <typename T>
class NoDupRandInt {
    std::uniform_int_distribution<T> u;
    std::set<T> except;
public:
    NoDupRandInt(const T &lb, const T &ub) : u(lb, ub) {}
    NoDupRandInt(const T &lb, const T &ub, const std::set<T> &set) : u(lb, ub), except(set) {}
    T operator() () {
        T res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

void outputVector(std::ofstream &fout, const auto &vec) {
    for (const auto &x : vec) {
        fout << x << " \n"[&x == &*vec.rbegin()];
    }
}

void output(int n, int m, int k, const auto &v1, const auto &v2, const auto &v3) {
    std::ofstream fout(getPath());
    fout << n << ' ' << m << ' ' << k << '\n';
    outputVector(fout, v1);
    outputVector(fout, v2);
    outputVector(fout, v3);
    fout.close();
}

void random_free(int n, int m, int k, int dtp, int ftp, int64 ttp) {
    std::vector<int> d(n), f(n);
    std::vector<int64> t(n);
    RandInt<int> rand_d(1, dtp), rand_f(1, ftp);
    RandInt<int64> rand_t(1LL, ttp);
    for (int i = 0; i < n; i++) {
        d[i] = rand_d(), f[i] = rand_f(), t[i] = rand_t();
    }
    output(n, m, k, d, f, t);
}

int main() {
    init();

    { // sample
        int n = 1, m = 2, k = 3;
        std::vector<int> df { 1,2,3 };
        std::vector<int64> t { 1LL,2LL,3LL };
        output(n, m, k, df, df, t);
    }

    int64 t_top = int64(1e12);

    { // hand: full
        int n = N, m = N, k = 1;
        std::vector<int> d(n), f(n, 1);
        std::iota(d.begin(), d.end(), 1);
        std::vector<int64> t(n);
        RandInt<int64> rand_t(1LL, t_top);
        for (int64 &x : t) {
            x = rand_t();
        }
        output(n, m, k, d, f, t);
    }

    // random
    constexpr int m_list[] = { 30, 300, 3000 };
    constexpr int k_list[] = { 30, 300, 3000 };
    constexpr int d_list[] = { 30, 300, 3000 };
    constexpr int f_list[] = { 10, 300 };

    for (int m : m_list) {
        for (int k : k_list) {
            for (int d : d_list) {
                for (int f : f_list) {
                    random_free(N, m, k, d, f, t_top);
                }
            }
        }
    }
    random_free(N, N, N, N, N, t_top);

    return 0;
}
```

# 干饭协会的捏脸师

## 题目描述

$hsy$ 是 $\text{EatMore}$ 干饭协会里著名的捏脸师，凡是曾经捏过的脸型，$hsy$ 都可以再为其他人捏出来（因为这个脸型从此以后就被 $hsy$ 记住了，这样一来，$hsy$ 想捏多少个就能捏多少个，随心所欲）。

<center>
<img src="resource/pinching.png" width="320px">
<br>
  <div style="color:gray">
    hsy 温柔捏脸ing...
  </div>
</center>

不过，如果 $hsy$ 从来没有捏过脸型 $x$，但是有顾客想要捏出这种脸型 $x$，那么 $hsy$ 需要先花费 $p_x$ 元设计脸型 $x$，然后再为这位顾客捏出该脸型，并且以后也就能够直接捏出脸型 $x$ 了（因为 $hsy$ 已经 记住了脸型 $x$）。

现在有 $m$ 位顾客，第 $i$ 位顾客想把自己的脸型 $u_i$ 捏成脸型 $v_i$，且顾客们的捏脸顺序可以由 $hsy$ 自由安排（保证每一顾客 $u_i \neq v_i$，不然就没必要来捏脸了）。

请问 $hsy$ 至少需要花费多少元才能把所有顾客捏成想要的脸型？

## 输入格式

本题包含多组测试数据。

第一行有一个整数 $T$ ($1 \leqslant T \leqslant 10000$)，表示一共有 $T$ 组测试数据。

对于每组测试数据：

- 第一行是空行。
- 第二行有 $2$ 个整数 $n, m$ ( $2 \leqslant n \leqslant 5 \cdot {10}^5, 1 \leqslant m \leqslant 5 \cdot {10}^5$ )， 表示不同的脸型数量为 $n$，要捏脸的顾客数量为 $m$。
- 第三行有 $n$ 个整数 $p_1, p_2, \cdots, p_n$ ( $1 \leqslant p_i \leqslant {10}^9$ )，表示设计脸型 $i$ 的花费为 $p_i$ 元。
- 接下来 $m$ 行的第 $i$ 行有 $2$ 个整数 $u_i, v_i$ ( $1 \leqslant u_i, v_i \leqslant n$，且 $u_i \neq v_i$ )，表示第 $i$ 位顾客想把当前脸型 $u_i$ 捏成目标脸型 $v_i$。

保证所有测试数据的脸型总数 $\displaystyle \sum n$ 、顾客总数 $\displaystyle \sum m$ 均不超过 $5 \cdot {10}^5$。

## 输出格式

输出共 $T$ 行，对于每组测试数据输出一行一个整数，表示 $hsy$ 把所有顾客捏成想要的脸型的最小花费。

## 样例 #1

### 样例输入 #1

```
1

3 3
3 7 9
1 2
2 3
3 1

5 4
5 6 7 8 9
2 1
3 1
4 1
1 5
```

### 样例输出 #1

```
3
9
```

## 提示

**样例 #1 的第 1 组测试数据:**

- 花费 $3$ 元设计脸型 $1$，此后 $hsy$ 能捏出脸型 $1$；
- 为第 $3$ 位顾客捏出脸型 $1$，此后 $hsy$ 能捏出脸型 $1,3$（第 $3$ 名顾客原来的脸型是 $3$）；
- 为第 $2$ 位顾客捏出脸型 $3$，此后 $hsy$ 能捏出脸型 $1,2,3$（第 $2$ 名顾客原来的脸型是 $2$）；
- 为第 $1$ 位顾客捏出脸型 $2$，此时所有顾客均已捏成目标脸型。

总花费 $3$ 元，可以证明这是最小花费。

**样例 #1 的第 2 组测试数据:**

- 花费 $9$ 元设计脸型 $5$，此后 $hsy$ 能捏出脸型 $5$；
- 为第 $4$ 位顾客捏出脸型 $5$，此后 $hsy$ 能捏出脸型 $1, 5$；
- 分别为第 $1,2,3$ 位顾客捏出脸型 $1$，此时所有顾客均已捏成目标脸型。

总花费 $9$ 元，可以证明这是最小花费。
```cpp
//=======================
//07-捏脸师-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

constexpr int N = 5e5, lim = 1e9, topcnt = 10000;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int, int>;

struct Testcase {
    std::vector<int> price;
    std::vector<PII> rela;
    Testcase() = default;
    Testcase(const std::vector<int> &_p, const std::vector<PII> &_r) : price(_p), rela(_r) {}
};

void output(std::ostream &fout, const std::vector<int> &price, const std::vector<PII> &rela) {
    fout << price.size() << ' ' << rela.size() << '\n';
    for (const int &x : price) {
        fout << x << " \n"[&x == &*price.rbegin()];
    }
    for (auto [u, v] : rela) {
        fout << u << ' ' << v << '\n';
    }
}

#include <iostream>
void output_all(const std::vector<Testcase> &tests) {
    if (tests.size() > topcnt) {
        std::cerr << "cases num limit exceeded (tests.size() == " << tests.size() << ")\n";
        return;
    }
    int sum_n = 0, sum_m = 0;
    for (const auto &[p, r] : tests) {
        sum_n += p.size();
        sum_m += r.size();
    }
    if (sum_n > N) {
        std::cerr << "n sum limit exceeded (sum of n == " << sum_n << ")\n";
        return;
    }
    if (sum_m > N) {
        std::cerr << "m sum limit exceeded (sum of m == " << sum_m << ")\n";
        return;
    }
    std::ofstream fout(getPath());
    fout << tests.size() << '\n';
    for (const auto &[price, rela] : tests) {
        output(fout, price, rela);
    }
    fout.close();
}

void output_sing(const std::vector<int> &price, const std::vector<PII> &rela) {
    std::vector<Testcase> test { Testcase(price, rela) };
    output_all(test);
}

std::vector<int> getprice(int n, int top) {
    std::vector<int> price(n);
    RandInt rand_p(1, top);
    for (int &p : price) {
        p = rand_p();
    }
    return price;
}

int main() {
    init();

    { // hand cases: sample
        std::vector<int> price{ 3,7,9 };
        std::vector<PII> rela{ {1,2}, {2,3}, {3,1} };
        output_sing(price, rela);
    }

    constexpr int top_list[2] = { 10, lim };

    // hand cases: complete graph
    for (int top : top_list) {
        int n = 707;
        std::vector<int> price = getprice(n, top);
        std::vector<PII> rela;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    rela.emplace_back(i, j);
                }
            }
        }
        output_sing(price, rela);
    }

    // hand cases: dag ensured
    for (int top : top_list) {
        std::vector<int> price = getprice(N, top);
        std::vector<PII> rela(N);
        std::set<PII> has;
        RandInt randInt(1, N);
        for (auto &[u, v] : rela) {
            do {
                u = randInt(), v = randInt();
                if (u > v) std::swap(u, v);
            } while (u == v || has.count(std::make_pair(u, v)));
            has.emplace(u, v);
        }
        output_sing(price, rela);
    }

    // hand cases: dag layers
    for (int top : top_list) {
        std::vector<int> price = getprice(N, top);
        std::vector<PII> rela;
        int layers = 500, sz = 1000;
        for (int i = 1; i < layers; i++) {
            RandInt randInt(1, i * sz);
            std::set<PII> has;
            for (int j = 0; j < 4*i; j++) {
                int u, v;
                do {
                    u = i * sz + randInt(), v = randInt();
                    if (u > v) std::swap(u, v);
                } while (u == v || has.count(std::make_pair(u, v)));
                has.emplace(u, v);
                rela.emplace_back(u, v);
            }
        }
        output_sing(price, rela);
    }

    // hand cases: intentionally copied
    for (int i = 1; i <= 2; i++) {
        std::string filename = "./special-in-" + std::to_string(i) + ".in";
        std::ifstream fin(filename);
        int n, m;
        fin >> n >> m;
        std::vector<int> price = getprice(n, 100);
        std::vector<PII> rela(m);
        for (auto &[u, v] : rela) {
            fin >> v >> u;
        }
        fin.close();
        output_sing(price, rela);
    }

    constexpr PII nm_list[] = { {50,50}, {N,N}, {50,1000}, {1000,50} };

    // random cases
    for (auto [n, m] : nm_list) {
        for (int top : top_list) {
            std::vector<Testcase> tests(N / std::max(n, m));
            for (auto &[price, rela] : tests) {
                price = getprice(n, top);
                rela = std::vector<PII>(m);
                RandInt randInt(1, n);
                for (auto &[u, v] : rela) {
                    do { u = randInt(), v = randInt(); } while (u == v);
                }
            }
            output_all(tests);
        }
    }

    return 0;
}
```

# 干饭协会的排队论

## 题目描述

“叮铃铃，叮铃铃……”。

随着干饭铃的响起，干饭协会的成员们冲向食堂。

<center>
<img src="resource/干饭啦.jpg" width="240px">
</center>

很快食堂窗口就排起了长队。大家边排队边看着窗口的菜单，讨论午餐吃什么。

没过多久，有同学就发现，因为前面有人比自己高，所以自己一直看不见菜单。所以这个同学想麻烦前面的同学一下低头，让他瞄一眼今天的菜单。

* 我们定义，对于某个参数 $k$，编号为 $i$ 的同学（身高为 $a_i$）看不见菜单，当且仅当在他前面有一个 编号为 $j(j<i)$ 的同学，他的身高 $a_j$ 满足 $a_j>a_i+k$。

现在有 $q$ 次询问，每一次询问给出两个整数 $pos,k$，表示编号为 $pos$ 的同学想知道，对于参数 $k$ 第 一个挡着他看菜单的同学的编号。

## 输入格式

第一行两个整数 $n,q$ ( $1\leqslant n,q\leqslant {10}^5$ )，分别表示队伍长度为 $n$ 和询问次数为 $q$。

第二行有 $n$ 个整数 $a_1,a_2,\cdots,a_n$ ( $1\leqslant a_i\leqslant {10}^9$ )，第 $i$ 个整数 $a_i$ 表示第 $i$ 个人的身高。

接下来 $q$ 行，每行 $2$ 个整数 $pos,k$ ( $1\leqslant pos\leqslant 10^5,0\leqslant k\leqslant 10^9$ )，表示编号为 $pos$ 的同学想知道，对于参数 $k$ 第一个挡着他看菜单的同学的编号。

## 输出格式

输出 $q$ 行，每行一个整数表示编号为 $pos$ 的同学想知道，对于参数 $k$ 第一个挡着他看菜单的同学的编 号。如果不存在这样的同学，输出 $-1$。

## 样例

输出：

```
6 5
1 9 1 9 8 10
1 1
4 5
1 4
3 7
5 0
```

输出：

```
-1
-1
-1
2
4
```


```cpp
//=======================
//08-排队论-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

using int64 = long long;

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

template <typename T=int>
class NoDupRandInt {
    std::uniform_int_distribution<T> u;
    std::set<T> except;
public:
    NoDupRandInt(const T &lb, const T &ub) : u(lb, ub) {}
    NoDupRandInt(const T &lb, const T &ub, const std::set<T> &set) : u(lb, ub), except(set) {}
    T operator() () {
        T res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int,int>;

void output(const std::vector<int> &height, const std::vector<PII> &query) {
    std::ofstream fout(getPath());
    fout << height.size() << '\n';
    for (const int &x : height) {
        fout << x << " \n"[&x == &*height.rbegin()];
    }
    for (auto [p, k] : query) {
        fout << p << ' ' << k << '\n';
    }
    fout.close();
}

int main() {
    init();

    constexpr int N = 1e5, lim = 1e9;

    { // sample
        std::vector<int> height { 1,9,1,9,8,10 };
        std::vector<PII> query { {1,1}, {4,5}, {1,4}, {3,7}, {5,0} };
        output(height, query);
    }

    { // hand: same height
        std::vector<int> height(N, lim);
        std::vector<PII> query(N);
        RandInt rand_pos(1, N), choose(0, 4);
        constexpr int GG = lim / 4;
        for (auto &[p, k] : query) {
            p = rand_pos();
            k = GG * choose();
        }
        output(height, query);
    }

    { // hand: very very long
        std::vector<int> height(N);
        RandInt randHigh(lim / 2, lim), randShort(1, 100);
        for (int i = 0; i < N / 10; i++) {
            height[i] = randHigh();
        }
        for (int i = N / 10; i < N; i++) {
            height[i] = randShort();
        }
        std::vector<PII> query(N);
        RandInt rand_pos(1, N), rand_logk(0, 29);
        for (auto &[p, k] : query) {
            p = rand_pos();
            k = (1 << rand_logk()) - 1;
        }
        output(height, query);
    }

    constexpr int h_lim_list[] = { 2, 10, 100, 10000, lim / 100, lim };

    for (int h_lim : h_lim_list) {
        std::vector<int> height(N);
        std::vector<PII> query(N);
        RandInt rand_h(1, h_lim), rand_pos(1, N), rand_k(0, h_lim);
        for (int &h : height) {
            h = rand_h();
        }
        for (auto &[p, k] : query) {
            p = rand_pos(), k = rand_k();
        }
        output(height, query);
    }

    return 0;
}
```

# 干饭协会的整向量

## 题目描述

最近干饭协会的会长 $lzl$ 痴迷于研究整向量。

整向量是这样定义的：

* 对于一个向量 $\vec{x} = (a, b)$，如果它的每一分量 $a, b$ 都是整数，那么我们就称之为 整向量。

这一天 $lzl$ 发现了三个非零整向量，它们分别是 $\vec{x} = (x_1,x_2), \vec{y} = (y_1,y_2), \vec{z} = (z_1,z_2)$。

$lzl$ 很好奇，是否存在整数 $a,b$ 使得 $a\vec{x}+b\vec{y}=\vec{z}$ 。

如果存在，你能告诉 $lzl$ 一组满足上述条件的整数 $a,b$ 吗。

## 输入格式

本题包含多组测试数据。

第一行一个整数 $t$ ( $1 \leqslant t \leqslant {10}^{5}$ )，表示有 $t$ 组测试数据。

接下 $t$ 行，每行 $6$ 个整数 $x_1,x_2,y_1,y_2,z_1,z_2$ ( $1 \leqslant |x_1|,|x_2|,|y_1|,|y_2|,|z_1|,|z_2| \leqslant {10}^{6}$ )，表示 $lzl$ 发现的三个非零整向量 $\vec{x} = (x_1,x_2), \vec{y} = (y_1,y_2), \vec{z} = (z_1,z_2)$（"非零" 就是保证 $|x|, |y|, |z| > 0$）。

## 输出格式

对于每组测试数据：

- 如果答案存在，第一行输出 `YES` ，然后第二行输出 $a,b$ 的值，且须满足 $|a|,|b| \leqslant 10^{12}$。可以证明，如果答案存在，那么就一定存在至少一组答案 $a, b$ 满足以上约束；如果有多组答案满足要求，输出任意一组即可。
- 如果答案不存在，直接输出一行 `NO`。

### 样例 #1

输入：

```
4
1 2 1 2 3 6
1 2 1 2 3 3
1 1 4 5 1 4
1 0 0 2 1 1
```

输出：

```
YES
1 1
NO
YES
-11 3
NO
```

## Note


```cpp
//=======================
//09-整向量-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <set>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

template <typename T=int>
class NoDupRandInt {
    std::uniform_int_distribution<T> u;
    std::set<T> except;
public:
    NoDupRandInt(const T &lb, const T &ub) : u(lb, ub) {}
    NoDupRandInt(const T &lb, const T &ub, const std::set<T> &set) : u(lb, ub), except(set) {}
    T operator() () {
        T res;
        do {
            res = u(e);
        } while (except.count(res));
        except.insert(res);
        return res;
    }
};

struct Testcase {
    int a, b, c, d, e, f;
    Testcase() = default;
    Testcase(int _a, int _b, int _c, int _d, int _e, int _f)
        : a(_a), b(_b), c(_c), d(_d), e(_e), f(_f) {}
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int,int>;

void output(const std::vector<Testcase> &tests) {
    std::ofstream fout(getPath());
    fout << tests.size() << '\n';
    for (auto [a, b, c, d, e, f] : tests) {
        fout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << ' ' << f << '\n';
    }
    fout.close();
}

int main() {
    init();

    constexpr int top_cnt = 1e5, lim = 1e6;

    { // sample
        std::vector<Testcase> tests{
            {1,2,1,2,3,6}, {1,2,1,2,3,3}, {1,1,4,5,1,4}, {1,0,0,2,1,1}
        };
        output(tests);
    }

    { // hand: small && on the same line
        constexpr int times_top(43478);
        RandInt rand_tm(-times_top, times_top);
        std::vector<Testcase> tests;
        int value_list[2*23];
        for (int i = 1; i <= 23; i++) {
            value_list[i*2-2] = i;
            value_list[i*2-1] = i;
        }
        for (int i : value_list) {
            for (int j : value_list) {
                for (int k : value_list) {
                    int tm = rand_tm();
                    tests.emplace_back(i, tm*i, j, tm*j, k, tm*k);
                }
            }
        }
        output(tests);
    }

    { // hand: always exists
        RandInt rand_abs(1, 5000), rand_sign(0, 1);
        auto randInt = [&rand_abs, &rand_sign] () -> int {
            return (rand_sign() ? +1 : -1) * rand_abs();
        };
        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests) {
            do {
                int x = randInt(), y = randInt();
                a = randInt(), b = randInt(), c = randInt(), d = randInt();
                e = a * x + c * y, f = b * x + d * y;
            } while (abs(e) + abs(f) == 0 || abs(e) > lim || abs(f) > lim);
        }
        output(tests);
    }

    constexpr int top_list0[] = { 500, lim };

    for (int top : top_list0) { // hand: contains zeros
        RandInt rand_abs(1, top), rand_type(0, 1);
        auto randInt = [&rand_abs, &rand_type] () -> int {
            return (rand_type() ? +1 : -1) * rand_abs();
        };
        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests) {
            a = 0, b = randInt(), c = 0, d = randInt();
            if (rand_type()) std::swap(a, b);
            if (rand_type()) std::swap(c, d);

            do {
                e = randInt(), f = randInt();
                if (rand_type()) {
                    if (rand_type()) e = 0;
                    else f = 0;
                }
            } while (abs(e) + abs(f) == 0);
        }
        output(tests);
    }

    constexpr int top_list[] = { 20, 500, 5000, lim };

    // random with limited upper bound
    for (int top : top_list) {
        RandInt randInt(-top, top);
        auto get_p = [&randInt] (int &a, int &b) -> void {
            do { a = randInt(), b = randInt(); } while (abs(a) + abs(b) == 0);
        };

        std::vector<Testcase> tests(top_cnt);
        for (auto &[a, b, c, d, e, f] : tests){
            get_p(a, b), get_p(c, d), get_p(e, f);
        }
        output(tests);
    }

    return 0;
}
```

# 干饭协会的欢乐树

## 题目描述

众所周知干饭协会会长 $lzl$ 是斗地主高手，他特别喜欢玩欢乐斗地主。其中欢乐豆就是欢乐斗地主中的货币 。

这一天 $lzl$ 不小心输掉了他所有的欢乐豆，所以他准备去欢乐树上摘欢乐豆。

<center>
<img src="resource/happy-tree.png" width="600px">
<br>
  <div style="color:gray">
    欢乐树
  </div>
</center>

为了简化问题，我们把欢乐树看作是一棵以 $1$ 为根节点的树。

这颗树共有 $n$ 个结点，编号为 $i$ 的结点上有 $a_i$ 个欢乐豆（由于某些原因，结点上的欢乐豆可以是负 数）。

为了让摘欢乐豆的过程看起来更欢乐，$lzl$ 将进行 $q$ 次操作。

每次操作都是以下两种操作中的一个：

* `1 pos d`：选定以编号为 $pos$ 的结点为根结点的子树，从该子树中的每个结点摘取 $d$ 个欢乐豆（如果 $d$ 为负数则表示把欢乐豆放回该结点）。
* `2 pos` ：查询编号为 $pos$ 的结点有多少个欢乐豆（可能为负）。

## 输入格式

第一行两个整数 $n,q$ ( $1 \leqslant n,q \leqslant {10}^5$ )，表示结点数为 $n$，操作次数为 $q$。

接二行 $n$ 个整数 $a_1, a_2, \cdots, a_n$ ( $-{10}^9 \leqslant a_i\leqslant {10}^9$ )，表示初始时 刻第 $i$ 个节点上的欢乐豆数量。

接下来 $n-1$ 行，每行两个整数 $u,v$ ( $1 \leqslant u, v \leqslant n$ )，表示节点 $u,v$ 之间有一条 边。保证输入的数据形成一棵树。

接下来 $q$ 行，每行表示下列两种操作之一：

* `1 pos d`：选定以编号为 $pos$ 的结点为根结点的子树，从该子树中的每个结点摘取 $d$ ($|d|\leqslant {10}^9$) 个欢乐豆（如果 $d$ 为负数则表示把 $|d|$ 欢乐豆放回该结点）。
* `2 pos` ：查询编号为 $pos$ 的结点有多少个欢乐豆（可能为负）。

## 输出格式

对于每一次查询操作，输出一个整数，表示查询结点的欢乐豆个数。

## 样例

输入：

```
5 6
7 3 7 8 0
1 2
1 3
3 4
3 5
2 3
1 1 -10
2 3
1 3 20
2 3
2 5
```

输出：

```
7
-3
17
10
```

```cpp
//=======================
//10-欢乐树-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct OP {
    int op, pos, d=-1;
    OP() = default;
    OP(int _op, int _pos, int _d=-1) : op(_op), pos(_pos), d(_d) {}
};

using PII = std::pair<int, int>;

void output(const std::vector<int> &a, const std::vector<PII> &e, const std::vector<OP> &q) {
    std::ofstream fout(getPath());
    fout << a.size() << ' ' << q.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [u, v] : e) {
        fout << u << ' ' << v << '\n';
    }
    for (auto [op, p, d] : q) {
        if (op == 1) fout << op << ' ' << p << ' ' << d << '\n';
        else fout << op << ' ' << p << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 2, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a, std::vector<PII> &e, std::vector<OP> &q) {
    auto map = shuffle_map(a.size());
    for (auto &[u, v] : e) u = map[u], v = map[v];
    for (auto &[_1, p, _2] : q) p = map[p];
    output(a, e, q);
}

constexpr int lim = 1e9;
RandInt rand_op(1, 2), rand_d(-lim, lim);

std::vector<int> get_a(int n) {
    std::vector<int> a(n);
    for (int &x : a) {
        x = rand_d();
    }
    return a;
}

std::vector<OP> get_op(int q, RandInt<int> &rand_p) {
    std::vector<OP> opr(q);
    for (auto &[op, p, d] : opr) {
        op = rand_op(), p = rand_p();
        if (op == 2) d = rand_d();
    }
    return opr;
}

int main() {
    init();

    { // samples
        std::vector<int> a{7,3,7,8,0};
        std::vector<PII> e{ {1,2}, {1,3}, {3,4}, {3,5}, {2,3} };
        std::vector<OP> op{ {1,1,-10}, {2,3}, {1,3,20}, {2,3}, {2,5} };
        output(a, e, op);
    }

    constexpr int N = 1e5;
    constexpr int n_list0[] = { 2,3,4,5,6,7,8,9, 100, 5000, N, N };

    // random: different n
    for (int n : n_list0) {
        std::vector<int> p(n + 1), a = get_a(n);
        RandInt rand_p(1, n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            p[now] = RandInt(1, now - 1)();
            e.emplace_back(now, p[now]);
        }
        auto ops = get_op(N, rand_p);
        shuffle_output(a, e, ops);
    }

    int n = N;

    // random: linked list shaped tree
    for (int c = 1; c <= 3; c++) {
        int tp_list[2] = { c, n };
        for (int tp : tp_list) {
            std::vector<int> p(n+1), a = get_a(n);
            RandInt rand_p(1, tp);
            std::vector<PII> e;
            for (int now = 2; now <= n; now ++) {
                p[now] = RandInt(std::max(1, now-c), now-1)();
                e.emplace_back(now, p[now]);
            }
            auto ops = get_op(N, rand_p);
            shuffle_output(a, e, ops);
        }
    }

    // random: "flower" shaped tree
    for (int t = 1; t <= 3; t++) {
        int tp_list[2] = { t, n };
        for (int tp : tp_list) {
            std::vector<int> p(n+1), a = get_a(n);
            RandInt rand_p(1, tp);
            std::vector<PII> e;
            for (int now = 2; now <= n; now ++) {
                p[now] = RandInt(1, std::min(now-1, t))();
                e.emplace_back(now, p[now]);
            }
            auto ops = get_op(N, rand_p);
            shuffle_output(a, e, ops);
        }
    }

    return 0;
}
```

# 干饭协会的签到题

## Description

<center>
<img src="resource/imagine-time.jpg" width="250px">
</center>

$\text{EatMore}$ 干饭协会在招纳新成员时，会对新人进行考核，要求其解决一道“入会资格题”。

不过，这道题总是那么简单，以至于大家都称之为签到题。

就在今天，干饭协会又开始招新啦！这道签到题是这样的：

给定一个包含 $n$ 个元素的数组 $a$，你可以对其进行如下操作：

- 从 $a$ 中任选两个数 $x, y$，将它们从 $a$ 中移除，并向 $a$ 中添加一个数 $x - y$。

显然，经过 $n-1$ 次操作后，数组 $a$ 恰好只剩一个元素；请问：最后这个元素最大是多少呢？

注意，从数组 $a$ 中移除 $x$ 时，尽管可能有多个值等于 $x$ 的元素，但是也**只删除其中一个**。

## Input

第一行有 $1$ 个整数，表示数组 $a$ 的初始大小 $n$ ( $2 \leqslant n \leqslant 5 \cdot {10}^{5}$ )。

第二行有 $n$ 个整数，表示数组 $a$ 的初始 $n$ 个元素 $a_1, a_2, \cdots, a_n$ ( $1 \leqslant a_i \leqslant {10}^9, 1 \leqslant i \leqslant n$ )。

## Output

输出一个整数，表示进行 $n-1$ 次操作后，数组 $a$ 中唯一一个元素的最大可能值。

## Sample #1

### Sample Input #1

```
2
1 1
```

### Sample Output #1

```
0
```

## Sample #2

### Sample Input #2

```
3
23333 114514 5201314
```

### Sample Output #2

```
5313495
```

## Sample #3

### Sample Input #3

```
4
1 1 1 1
```

### Sample Output #3

```
2
```

## Hint

**样例 #1:**
初始数组 $a$ 中仅含两个数 $1$ 和 $1$，故进行如下操作：
先把它们从 $a$ 中移出，此时 $a = []$，再把 $1-1=0$ 插入到 $a$ 中，于是 $a = [1]$，此时已经无法再操作了，因此答案为 $1$。
```cpp
//=======================
//11-签到题-generator.cpp
#include <fstream>
#include <random>
#include <vector>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

void output(const std::vector<int> &a) {
    std::ofstream fout(getPath());
    fout << a.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    fout.close();
}

int main() {
    init();

    constexpr int lim = 1e9;

    { // samples
        output(std::vector<int>{1,1});
        output(std::vector<int>{23333,114514,5201314});
        output(std::vector<int>{1,1,1,1});
    }

    // random: small n
    for (int n = 2; n <= 10; n++) {
        constexpr int top_list[] = { 2, 1000, lim / 1000, lim };
        for (int top : top_list) {
            std::vector<int> tmp(n);
            RandInt randInt(1, top);
            for (int &x : tmp) {
                x = randInt();
            }
            output(tmp);
        }
    }

    constexpr int N = 5e5;

    // random: small & large values
    constexpr std::pair<int,int> bt_list[] = { {1,2}, {1,50}, {1,500}, {1,lim}, {lim/2,lim} };

    for (auto [bot, top] : bt_list) {
        std::vector<int> tmp(N);
        RandInt randInt(bot, top);
        for (int &x : tmp) {
            x = randInt();
        }
        output(tmp);
    }

    return 0;
}
```

# 干饭协会的金币树

## Description

$\text{EatMore}$ 干饭协会的会员大食堂门口有一颗金币树，树上每一个结点都有一定量的金币。

一个正午，富哥们 $\text{chd}$ 正打算来食堂干饭，但他突然看着这棵树出了神——

$\text{chd}$ 认为，只有**从根节点开始到每个叶子结点的路径上的金币数量之和**都相等的树才是吉利的。

$\text{chd}$ 想把这棵树变成吉利的，但他只能在任意结点上添加金币，不能减少、移动树上已有的金币。

$\text{chd}$ 想知道，最少要花费多少个金币才能使得这棵树是吉利的？当然也可以不花费金币，此时输出 $0$ 即可。

## Input

第一行有 $1$ 个整数 $n$ ( $2 \leqslant n \leqslant 5 \cdot {10}^{5}$ )，表示金币树的结点数 $n$。

第二行有 $n$ 个整数 $a_1, a_2, \cdots, a_n$ ( $1 \leqslant a_i \leqslant {10}^9, 1 \leqslant i \leqslant n$ )，表示初始时刻，编号为 $i$ 的结点有 $a_i$ 个金币。

接下来 $n - 1$ 行中的第 $i$ 行有 $2$ 个整数 $u_i, v_i$ ( $1 \leqslant u_i, v_i \leqslant n$ )，表 示结点 $u_i, v_i$ 之间有一条边。

数据保证这些边构成一棵树，并规定编号为 $1$ 的结点是根节点。

## Output

输出一个整数，表示 $\text{chd}$ 最少需花费的金币数。

## Sample #1

### Sample Input #1

```
5
1 1 1 1 1
2 1
3 1
4 1
5 1
```

### Sample Output #1

```
0
```

## Sample #2

### Sample Input #2

```
4
100 1 60 60
1 2
1 3
3 4
```

### Sample Output #2

```
119
```

```cpp
//=======================
//12-金币树-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int, int>;

void output(const std::vector<int> &a, const std::vector<PII> &e) {
    std::ofstream fout(getPath());
    fout << a.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [u, v] : e) {
        fout << u << ' ' << v << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 2, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a, std::vector<PII> &e) {
    auto map = shuffle_map(a.size());
    for (auto &[u, v] : e) {
        u = map[u], v = map[v];
    }
    output(a, e);
}

constexpr int lim = 1e9;
RandInt rand_d(1, lim);

std::vector<int> get_a(int n) {
    std::vector<int> a(n);
    for (int &x : a) {
        x = rand_d();
    }
    return a;
}

int main() {
    init();

    { // samples
        { // sample #1
            std::vector<int> a(5, 1);
            std::vector<PII> e{ {2,1}, {3,1}, {4,1}, {5,1} };
            output(a, e);
        }
        { // sample #2
            std::vector<int> a{ 100, 1, 60, 60 };
            std::vector<PII> e{ {1,2}, {1,3}, {3,4} };
            output(a, e);
        }
    }

    constexpr int N = 5e5;
    constexpr int n_list0[] = { 2,3,4,5,6,7,8,9, 5000, N };

    // random: different n
    for (int n : n_list0) {
        std::vector<int> a = get_a(n);
        RandInt rand_p(1, n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(1, now - 1)();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    int n = N;

    // random: linked list shaped tree
    for (int c = 1; c <= 3; c+=2) {
        std::vector<int> a = get_a(n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(std::max(1, now-c), now-1)();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    // random: "flower" shaped tree
    for (int t = 1; t <= 3; t+=2) {
        std::vector<int> a = get_a(n);
        std::vector<PII> e;
        for (int now = 2; now <= n; now ++) {
            int pnow = RandInt(1, std::min(now-1, t))();
            e.emplace_back(now, pnow);
        }
        shuffle_output(a, e);
    }

    return 0;
}
```

# 干饭协会的金手帕

## 题目描述

相传干饭协会有一个祖传的金手帕，每一次出去聚餐都会带上它，并用它来做游戏，据说会带来好运。

<center>
<img src="resource/gold-handkerchief.png" width="320px">
<br>
  <div style="color:gray">
    金手帕
  </div>
</center>

某次比赛后，干饭协会全体成员一起去聚餐。由于菜还没有上齐，大家就开始玩丢手帕的游戏了。

游戏规则是这样的：

* 有  $ n $ 名成员参加游戏，编号从 $ 1 $ 到 $ n $ 。

* 如果手帕在编号为 $ x $ 的人手中，他就会把手帕传给编号为 $ a_x $ 的人。

现在 $ pyq $ 哥哥有 $ q $ 个想法，每个想法表示为两个数字 $ pos,d $ 表示 $ pyq $ 哥哥想知道起始位置为 $ pos $ ，传递 $ d $ 次之后手帕在谁手中。

由于 $ pyq $ 哥哥和吉吉国王一样急，所以你能在 $ 1s $ 内告诉他答案吗？

## 输入格式

 第一行两个整数 $ n,q $ ( $1 \leqslant n, q \leqslant {10}^5$ )，表示有 $n$ 个人做游戏，$pyq$ 哥哥有 $q$ 个想法。

第二行 $ n $ 个整数 $a_1, a_2, \cdots, a_n$ ( $1 \leqslant a_i \leqslant n, 1 \leqslant i \leqslant n$ )，表示第 $i$ 个人下一次会把手帕传给 $a_i$。

接下来 $ q $ 行每行两个整数 $pos,k$ ( $1 \leqslant pos \leqslant n,1 \leqslant k \leqslant {10}^9$ )，表示 $ pyq $ 哥哥想知道起始位置为 $ pos $ ，传递 $ d $ 次之后手帕在谁手中。

## 输出格式

输出 $ q $ 行，每行一个整数表示答案。

## 样例

输入：

```
6 3
2 3 1 1 6 5
3 5
4 2
5 999999
```

输出：

```
2
2
6
```

```cpp
//=======================
//13-金手帕-generator.cpp
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

using PII = std::pair<int, int>;

void output(const std::vector<int> &a, const std::vector<PII> &ask) {
    std::ofstream fout(getPath());
    fout << a.size() << ' ' << ask.size() << '\n';
    for (const int &x : a) {
        fout << x << " \n"[&x == &*a.rbegin()];
    }
    for (auto [p, k] : ask) {
        fout << p << ' ' << k << '\n';
    }
    fout.close();
}

std::vector<int> shuffle_map(int n) {
    std::vector<int> mp(n+1);
    std::iota(mp.begin(), mp.end(), 0);
    std::random_shuffle(mp.begin() + 1, mp.end());
    return mp;
}

void shuffle_output(const std::vector<int> &a0, std::vector<PII> ask) {
    int n = a0.size();
    auto map = shuffle_map(n);
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        a[map[i+1]] = map[a0[i]];
    }
    for (auto [p, _] : ask) {
        p = map[p];
    }
    output(a, ask);
}

int main() {

    constexpr int N = 1e5, lim = 1e9;

    RandInt rand_k(1, lim);

    { // sample
        std::vector<int> a{ 2,3,1,1,6,5 };
        std::vector<PII> ask{ {3,5}, {4,2}, {5,999999} };
        output(a, ask);
    }

    { // hand: big cycle
        std::vector<int> a(N);
        std::iota(a.begin(), a.end(), 2);
        *a.rbegin() = 1;
        int c0 = 1, k0 = N / 2;
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = c0++, k = k0;
        }
        output(a, ask);
        shuffle_output(a, ask);
    }

    { // hand: self-cycle
        std::vector<int> a(N);
        std::iota(a.begin(), a.end(), 1);
        int c0 = 1;
        RandInt rand_k(1, lim);
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = c0++, k = rand_k();
        }
        output(a, ask);
    }

    // random 1
    for (int i = 0; i < 3; i++) {
        std::vector<int> a(N);
        RandInt rand_p(1, N), rand_k(1, lim);
        for (int &x : a) {
            x = rand_p();
        }
        std::vector<PII> ask(N);
        int c0 = 1;
        for (auto &[p, k] : ask) {
            p = c0++, k = rand_k();
        }
        output(a, ask);
    }

    // random 2
    for (int i = 0; i < 3; i++) {
        std::vector<int> a(N);
        RandInt rand_p(1, N), rand_k(1, lim);
        for (int &x : a) {
            x = rand_p();
        }
        std::vector<PII> ask(N);
        for (auto &[p, k] : ask) {
            p = rand_p(), k = rand_k();
        }
        output(a, ask);
    }

    return 0;
}
```

# 干饭协会的魔法棒

## 题目描述

有一天，$\text{EatMore}$ 干饭协会会长 $\text{lzl}$ 突然不见了！大家都非常着急！

一查监控发现，原来是 $\text{lzl}$ 拿书时，书柜略微倾斜，柜顶的魔法棒滚下来，把 $\text{lzl}$ 变成了白纸上的字符串 `lzl`。

但 $\text{lzl}$ 似乎因为魔法棒的非正常释放，意外地掌握了操控字符串 $s$ 的能力，正愁无题可出的 $\text{lzl}$ 突然有了灵感，于是 ta 将计就计，在不经意间逃离了这张白纸躲了起来，然后在白纸的顶部写下了 下面这道题：

- 下面是一个长度为 $n$ 且仅含小写字母的字符串 $s$，接下来会发生 $m$ 个事件，事件分为以下两种：

  - `change p c`：将 $s$ 的第 $p$ 个字符 $s_p$ 改为 $c$，其中 $c$ 为单个小写字母；
  - `count-lzl l r`：输出在 $s$ 的子串 $s_l s_{l+1} \cdots s_{r}$ 中不同的等于 `lzl` 的子序列个数 。

你能解决这道题吗？只要你解决了这道题，$\text{lzl}$ 就会忍不住发出“哇喔”的声音从而无所遁形。

作为提示，字符串 $t$ 的子串是 $t$ 中某一段连续的字符串，而子序列不必连续。

例如，记字符串 $t$ 为 `csust`，那么 `sus` 是 $t$ 的子串和子序列，`css` 是 $t$ 的子序列但并非子串，`stc` 既不是 $t$ 的子序列也不是 $t$ 的子串。

## 输入格式

第一行包含一个长度为 $n$ 的仅含小写字母的字符串 $s$ ( $3 \leqslant n \leqslant 2\cdot {10}^5$ )， 表示初始字符串。

第二行有 $1$ 个整数 $m$ ( $1 \leqslant m \leqslant 2 \cdot {10}^5$ )，表示总事件数。

接下来 $m$ 行，每行表示下面两者中的一个事件：

- `change p c`：将 $s$ 的第 $p$ 个字符 $s_p$ 改为 $c$ ( $1 \leqslant p \leqslant n$，保证 $c$ 为单个小写字母 ) ；
- `count-lzl l r`：输出在 $s$ 的子串 $s_l s_{l+1} \cdots s_{r}$ 中不同的等于 `lzl` 的子序列个数 ( $1 \leqslant l \leqslant r \leqslant n$ )。

保证对于每组测试数据，`count-lzl` 事件至少出现 $1$ 次。

## 输出格式

对于所有 `count-lzl` 事件，输出一行一个整数表示答案。

## 样例 #1

### 样例输入 #1

```
lllll
5
change 3 z
count-lzl 2 4
count-lzl 1 5
change 3 x
count-lzl 1 5
```

### 样例输出 #1

```
1
4
0
```

```cpp
//=======================
//14-魔法棒-generator.cpp
#include <fstream>
#include <random>
#include <vector>

std::mt19937 e;
void init() {
    e.seed(std::random_device()());
}

template <typename T=int>
class RandInt {
    std::uniform_int_distribution<T> u;
public:
    RandInt(const T &lb, const T &ub) : u(lb, ub) {}
    T operator() () {
        return u(e);
    }
};

using RandChar = RandInt<char>;

std::string getPath() {
    static int num = 0;
    num ++;
    return "./data/" + std::to_string(num) + ".in";
}

struct OPR {
    int op_type, x, y;
    OPR() = default;
    std::string to_string() const {
        if (op_type == 0) {
            return "change " + std::to_string(x) + " " + char(y);
        } else {
            return "count-lzl " + std::to_string(x) + " " + std::to_string(y);
        }
    }
};

void output(const std::string &s, const std::vector<OPR> &op) {
    std::ofstream fout(getPath());
    fout << s << '\n';
    fout << op.size() << '\n';
    for (const auto &item : op) {
        fout << item.to_string() << '\n';
    }
    fout.close();
}

RandInt rand_op(0, 1);

template <typename T>
void random_free(int n, int m, T rand_c) {
    std::string s(n, '\0');
    for (char &c : s) {
        c = rand_c();
    }
    std::vector<OPR> op(m);
    RandInt rand_p(1, n);
    for (auto &[op, x, y] : op) {
        op = rand_op();
        if (op == 0) {
            x = rand_p(), y = rand_c();
        } else {
            x = rand_p(), y = rand_p();
            if (x > y) std::swap(x, y);
        }
    }
    output(s, op);
}

int main() {

    constexpr int N = 2e5;

    // tools
    RandInt c2(0, 1), c4(0, 3);
    RandChar rand_c('a', 'z');
    auto rand_lz = [&c2] () -> char {
        constexpr char chrs[2] = { 'l', 'z' };
        return chrs[c2()];
    };
    auto rand_ablz = [&c4] () -> char {
        constexpr char chrs[4] = { 'a', 'b', 'l', 'z' };
        return chrs[c4()];
    };

    { // sample
        std::string s = "lllll";
        std::vector<OPR> op = { {0,3,'z'}, {1,2,4}, {1,1,5}, {0,3,'x'}, {1,1,5} };
        output(s, op);
    }

    { // hand: large answer
        std::string s(N, 'l');
        for (int i = N / 3; i < N * 2 / 3; i++) {
            s[i] = 'z';
        }
        std::vector<OPR> op(N);
        RandInt rand_p(1, N);
        for (auto &[op, x, y] : op) {
            op = rand_op();
            if (op == 0) {
                x = rand_p(), y = rand_lz();
            } else {
                x = 1, y = N;
            }
        }
        output(s, op);
    }

    // random
    for (int i = 0; i < 3; i++) {
        random_free(N, N, rand_c);
        random_free(N, N, rand_lz);
        random_free(N, N, rand_ablz);
    }

    return 0;
}
```