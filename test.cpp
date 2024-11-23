/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1522034833367212 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N;  std::cin >> N;

        std::vector<int> bonus(N+1);
        for (auto &i: bonus | rgs::drop(1))  std::cin >> i;

        // F[i][j] 处理到第 i 只怪物，总共击败模 2 余 j 只，最高分数
        std::vector<std::vector<ll>> F(N+1, std::vector<ll>(2));
        F[0][1] = -infLL;
        auto ans = -infLL;
        for (auto i: range(1, N+1)) {
            for (auto j: {0, 1}) {
                // 放走
                chkMax(F[i][j], F[i-1][j]);
                // 攻击
                chkMax(F[i][j], F[i-1][j^1] + (j? 1: 2) * bonus[i]);
                // debug std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
                chkMax(ans, F[i][j]);
            }
        }

        std::cout << ans << std::endl;
    }
}

int main() {
    initDebug;
    Solution_1522034833367212::solve();
    return 0;
}