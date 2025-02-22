#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
#define FILE_NAME "xorsum"
using i32 = int32_t; using i64 = int64_t;
using u32 = uint32_t; using u64 = uint64_t;

char constexpr endl = '\n';

/**
以样例为例。拆位考虑：
0001
0010
1010
0001
第 0 位上分别为 1100
用 F[i][j][k] 表示前 i 个数，一共选出 j 个，最后算出 k 的方案数（k 为 0 或 1）
F[N][j][1] 种方案在这一位上的贡献为 1，乘以位值即可
复杂度 N*N*logN
*/
namespace Solution {
    i32 constexpr mod = 998244353;
    i32 constexpr maxBits = 31;
    auto qpow(i64 a, i64 b, i32 const mod) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    i32 constexpr maxN = 3001;
    // F[i][j][k]: 在本位上，前 i 个数，选出 j 个，异或和为 k 的方案数（k = 0 或 1）
    std::array<std::array<i32, 2>, maxN> F;
    std::array<i32, maxN> nums;
    std::array<i32, maxN> ans;
    std::array<i32, maxN> cur_bit;

    auto near_mod(i32 x) {
        if (x > mod)  x -= mod;
        return x;
    }
    auto solve() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        // std::freopen(FILE_NAME ".in", "r", stdin);
        // std::freopen(FILE_NAME ".out", "w", stdout);

        i32 N;  std::cin >> N;
        for (i32 i = 1; i <= N; i++)  std::cin >> nums[i];

        for (i32 d = 0; d < maxBits; d++) {
            for (i32 i = 1; i <= N; i++) {
                cur_bit[i] = (nums[i] >> d) & 1;
            }

            std::memset(F.begin(), 0, sizeof(F));

            F[0][0] = 1;
            for (i32 i = 1; i <= N; i++) {
                for (i32 j = N; j >= 0; j--) {
                    for (auto k: {0, 1}) {
                        if (j != 0)  F[j][k] = near_mod(F[j][k] + F[j-1][k ^ cur_bit[i]]);
                    }
                }
            }

            for (i32 j = 1; j <= N; j++) {
                ans[j] = near_mod(ans[j] + (static_cast<i64>(F[j][1]) << d) % mod);
            }
        }

        for (i32 j = 1; j <= N; j++) {
            std::cout << ans[j] << endl;
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}