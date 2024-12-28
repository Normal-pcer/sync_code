/**
 * @link https://www.luogu.com.cn/problem/P6883
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1515375577415987 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector C(N, std::vector<int>(N));
        for (auto &line: C) {
            for (auto &item: line) {
                std::cin >> item;
            }
        }
        for (auto mid: range(N - 1)) {
            for (auto begin: range(mid)) {
                for (auto end: range(mid + 1, N)) {
                    chkMin(C[begin][end], C[begin][mid] + C[mid][end]);
                }
            }
        }
        std::vector<int> F(1 << N, inf);  // 状态为 i，最小代价
        F[(1 << N) - 1] = 0;
        auto ans = inf;
        for (auto i: range(1 << N) | views::reverse) {
            for (auto j: range(N)) {
                if (not (i & 1 << j))  continue;
                for (auto k: range(N)) {
                    if (not (i & 1 << k))  continue;
                    if (j == k)  continue;

                    auto new_st = i ^ (1 << k);
                    chkMin(F[new_st], F[i] + C[k][j]);
                }
            }
            if (std::popcount((unsigned)i) <= K) {
                chkMin(ans, F[i]);
            }
        }
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1515375577415987::solve();
    return 0;
}
