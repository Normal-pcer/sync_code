/**
 * @link https://www.luogu.com.cn/problem/P2734
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6050919389568924 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i32> a(n);
        for (auto &x: a) std::cin >> x;

        // F[i][j] 表示面对区间 [i, j] 的人在最优策略下可以获得的分数差
        std::vector F(n, std::vector<i32>(n, -0x3f3f3f3f));
        for (i32 i = 0; i < n; i++) {
            F[i][i] = a[i];
        }

        for (i32 l = 2; l <= n; l++) {
            for (i32 i = 0; i < n; i++) {
                auto j = i + l - 1;
                if (j >= n) break;
                chkMax(F[i][j], -F[i][j - 1] + a[j]);
                chkMax(F[i][j], -F[i + 1][j] + a[i]);
            }
        }

        auto diff = F[0][n - 1];
        auto sum = std::accumulate(a.begin(), a.end(), (i32)0);
        auto ans1 = (sum + diff) / 2;
        auto ans2 = (sum - diff) / 2;
        std::cout << ans1 << " " << ans2 << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6050919389568924::solve();
    return 0;
}
