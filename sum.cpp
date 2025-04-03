#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"
#define FILENAME "sum"

char constexpr endl = '\n';

/*
显然要求什么东西的前缀和。
来把这个展开。例如 1...6

123
124
125
126
134
135
136
145
146
156

234
235
236
245
246
256

345
346
356

456

所以我们需要求 2*, 3*, 4*, 5*, ... 的前缀和
先求一下 1, 2, 3, ... 的前缀和

然后直接推就行
*/
namespace Solution {
    i32 constexpr mod = 1e9 + 7;
    auto solve2() -> void {
        i32 N; std::cin >> N;
        std::vector<i32> a(N);
        for (auto &x: a) std::cin >> x;
        i32 ans = 0;
        for (i32 i = 0; i < N; i++) {
            for (i32 j = i + 1; j < N; j++) {
                for (i32 k = j + 1; k < N; k++) {
                    ans = (ans + (static_cast<i64>(a[i]) * a[j] % mod * a[k] % mod)) % mod;
                }
            }
        }
        std::cout << static_cast<i64>(ans) * 6 % mod << endl;
    }
    auto addMod(i32 x, i32 y) -> i32 {
        auto res = x + y;
        if (res >= mod) res -= mod;
        return res;
    }
    auto mulMod(i32 x, i32 y) -> i32 {
        return static_cast<i32>(static_cast<i64>(x) * y % mod);
    }
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::vector<i32> a(N);
        for (auto &x: a) std::cin >> x;
        
        std::vector<i32> a_ps(N);
        std::partial_sum(a.begin(), a.end(), a_ps.begin(), addMod);

        // 1*, 2*, 3*, 4* 的前缀和
        // 1*：a1 * (a2 + a3 + a4 + a5 + ...)
        // 2*: a2 * (a3 + a4 + a5 + ...)
        std::vector<i32> aa_ps(N - 1);
        for (i32 i = 0; i < N - 1; i++) {
            aa_ps[i] = a_ps[N - 1] - a_ps[i];
            if (aa_ps[i] < 0) aa_ps[i] += mod;
            aa_ps[i] = mulMod(aa_ps[i], a[i]);
        }
        std::partial_sum(aa_ps.begin(), aa_ps.end(), aa_ps.begin(), addMod);

        i32 ans = 0;
        for (i32 i = 0; i < N - 2; i++) {
            auto cur = aa_ps[N - 2] - aa_ps[i];
            if (cur < 0) cur += mod;
            cur = mulMod(a[i], cur);

            ans = addMod(ans, cur);
        }
        std::cout << mulMod(ans, 6) << endl;
    }
}

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution::solve();
}
