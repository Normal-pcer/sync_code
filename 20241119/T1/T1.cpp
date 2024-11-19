#include <bits/stdc++.h>
#pragma GCC optimize(3)
#pragma GCC optimize("inline")

using ll = long long;

namespace Solution {

    constexpr inline auto pow(ll a, ll b, int p) {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % p) if (b&1) res = res * a % p;
        return res;
    }

    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
#ifndef ONLINE_JUDGE
        std::freopen("T1.in", "r", stdin);
        std::freopen("T1.out", "w", stdout);
#endif
        int M, Q, P;
        const constexpr int _Y = 1e7+5;
        std::cin >> M >> Q >> P;
        std::vector<std::pair<int, int>> f;  // 多项式 f；每一项为系数和指数

        for (auto i = 1; i <= M; i++) {
            int x, y;
            std::cin >> x >> y;
            f.push_back({x, y});
        }

        // std::array<int, 100'005> calc_memory;
        auto calc = [&](int x) -> int {
            // if (0 <= x and x < 100'005 and calc_memory[x])  return calc_memory[x];
            auto res = 0LL;
            for (auto [a, b]: f) {
                res += (long long)a * pow(x, b, P);
                res %= P;
            }
            return res;
        };

        // 考虑倍增
        auto constexpr lg = std::__lg(_Y) + 1;  // 需要的步数
        std::vector<std::vector<int>> F(P, std::vector<int>(lg));  // F[i][j] 表示从 i 迭代 2^j 次的值

        // 首先，预处理出每一个数向下跳一步是谁
        for (auto x = 0; x < P; x++) {
            F[x][0] = calc(x);
        }

        // 预处理倍增数组
        // O(P logY)
        for (auto j = 1; j < lg; j++) {
            for (auto i = 0; i < P; i++) {
                auto half = F[i][j-1];  // 迭代一半次数
                F[i][j] = F[half][j-1];  // 完成迭代
            }
        }

        const auto _DIGIT_MAX = 24;
        for (auto _ = 1; _ <= Q; _++) {
            int x, y;
            std::cin >> x >> y;

            auto res = x % P;
            for (auto digit = 0; digit < _DIGIT_MAX; digit++) {
                auto mask = 1 << digit;
                if (mask & y)  res = F[res][digit];  // 迭代 2^digit 次
            }

            // 单次查询复杂度 O(logY)

            std::cout << res << '\n';
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}