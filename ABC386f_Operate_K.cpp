#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
#define FILE_NAME "edit"
using i32 = int32_t; using i64 = int64_t;
using u32 = uint32_t; using u64 = uint64_t;

char constexpr endl = '\n';

/**
类似最长公共子序列的 DP。
F[i][j] 表示 S 的前 i 个字符与 T 的前 j 个字符匹配需要的最小操作次数。
F[i][j] <- F[i][j-1] + 1（删除）
F[i][j] <- F[i-1][j] + 1（新增）
F[i][j] <- F[i-1][j-1] + !!(S[i] != S[j]) （修改或不变）

K <= 20，i 和 j 的差距大于 K 就没有可能了，所以复杂度可以为 O(N*K)
状态换成 G[i][del + outset]，del 表示 i - j
*/
namespace Solution {
    i32 constexpr inf = 0x3f3f3f3f;
    template <typename T>  auto chkMax(T &base, T const &other) -> T & { return (base = std::max(base, other)); }
    template <typename T>  auto chkMin(T &base, T const &other) -> T & { return (base = std::min(base, other)); }
    // 多测
    auto solve() -> void {
        i32 K;  std::cin >> K;
        std::string S, T;  std::cin >> S >> T;
        S.insert(S.begin(), '_'), T.insert(T.begin(), '_');  // 下标从 1 开始

        if (std::abs<i32>(S.size() - T.size()) > K) {
            std::cout << "No" << std::endl;
            return;
        }

        auto offset = K;
        auto j_size = 2 * K + 1;
        auto i_size = S.size() + 1;
        std::vector<std::vector<i32>> F(i_size, std::vector<i32>(j_size, inf));

        auto getF = [&](i32 i, i32 j) -> i32 & {
            auto del = j - i;
            assert(-K <= del and del <= K);

            return F[i][del + offset];
        };
        getF(0, 0) = 0;
        i32 N = S.size() - 1;
        for (i32 i = 0; i <= N; i++) {
            for (i32 del = -K; del <= K; del++) {
                auto j = i + del;
                if (j < 0 or static_cast<size_t>(j) >= T.size())  continue;
                
                if (del != -K and j - 1 >= 0)  chkMin(getF(i, j), getF(i, j - 1) + 1);
                // std::printf("getF(%d, %d) = %d\n", i, j, getF(i, j));
                if (del != K and i - 1 >= 0)  chkMin(getF(i, j), getF(i - 1, j) + 1);
                // std::printf("getF(%d, %d) = %d\n", i, j, getF(i, j));
                if (i != 0 and j != 0)  chkMin(getF(i, j), getF(i - 1, j - 1) + (S[i] != T[j]));
                // std::printf("getF(%d, %d) = %d\n", i, j, getF(i, j));
            }
        }

        auto ans = getF(S.size() - 1, T.size() - 1);
        if (ans > K) {
            std::cout << "No" << endl;
        } else {
            std::cout << "Yes" << endl;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    // std::freopen(FILE_NAME ".in", "r", stdin);
    // std::freopen(FILE_NAME ".out", "w", stdout);

    i32 N = 1;
    while (N --> 0) {
        Solution::solve();
    }
    return 0;
}