/**
 * @link https://www.luogu.com.cn/problem/P6323
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1440140571231788 {
    const int mod = 1e9+7;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, C;  std::cin >> N >> C;
        std::vector F(N+1, std::vector<int>(C+1));
        std::vector<int> G(C+2);
        F[0][0] = 1;
        for (auto i: range(1, N+1)) {
            std::partial_sum(F[i-1].begin(), F[i-1].end(), G.begin(), lam(x, y, (x + y) % mod));
            for (auto j: range(C+1)) {
                const auto sum_end = std::min(j+1, i);
                auto sum = 0;
                sum = (G[j] - (j - sum_end < 0? 0: G[j - sum_end]) + mod) % mod;
                F[i][j] = sum;
            }
        }
        auto ans = F[N][C];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1440140571231788::solve();
    return 0;
}
