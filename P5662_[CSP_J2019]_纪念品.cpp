/**
 * @link https://www.luogu.com.cn/problem/P5662
 */

#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_2415534212037238 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int T, N, M;  std::cin >> T >> N >> M;
        const int _M = 10008;
        std::vector price(T+1, std::vector<int>(N+1));  // price[i][j]：第 i 天 j 物品的价格
        for (auto &line: price | views::drop(1)) {
            for (auto &item: line | views::drop(1)) {
                std::cin >> item;
            }
        }
        // G[i][j][k]：第 i 天，前 j 种物品，剩余现金为 k，第二天最大现金数（天数从 1 开始）
        // G[i][j][k] = G[i][j-1][k + price[i][j]] + price[i+1][j]
        // G[i][j][k] = G[i][j-1][k]
        // G[i][0][F[i-1][N][x]] = F[i-1][N][x]
        // F[i][k]：对于当前遍历到的 j，G[i][j][k]
        std::vector<std::vector<int>> F(T+1);
        F[0].resize(M+1, -inf), F[0][M] = M;
        for (auto i: range(1, T+1)) {
            auto max_k = 0;
            for (auto x: F[i-1])  chkMax(max_k, x);
            assert((int)F[i].size() <= max_k), F[i].resize(max_k+1, -inf);
            for (auto x: F[i-1])  chkMax(F[i][F[i-1][x]], F[i-1][x]);
            for (auto j: range(1, N+1)) {
                for (auto k: range(1, (int)F[i].size() - price[i][j])) {
                    chkMax(F[i][k], F[i][k + price[i][j]] + price[i+1][j]);
                }
            }
        }

        // auto ans = F[T][N]
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2415534212037238::solve();
    return 0;
}
