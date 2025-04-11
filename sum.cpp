/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "sum"

namespace Solution_7637820722732645 {
    auto solve() -> void {
        i32 n, m, k;
        std::cin >> n >> m >> k;

        if (k == 0) k = 1;

        std::vector<i32> a(n + 1);
        for (i32 i = 1; i <= n; i++) {
            std::cin >> a[i];
        } 

        // f[c][i] 表示取了 c 个数，最后一个数位置为 i。
        i64 constexpr inf = 0x3f3f3f3f3f3f3f3fLL;
        std::vector<std::vector<i64>> f(m + 1, std::vector<i64>(n + 1, -inf));
        for (i32 i = 1; i <= n; i++) {
            chkMax<i64>(f[1][i], a[i]);
        }
        for (i32 c = 2; c <= m; c++) {
            // 合法 f[c - 1][j] 的最大值
            i64 max = -inf;
            for (i32 i = 1; i <= n; i++) {
                // i - k 现在可以成为答案
                if (i - k >= 1) chkMax(max, f[c - 1][i - k]);
                chkMax(f[c][i], max + a[i]);
            }
        }

        auto ans = *std::max_element(f[m].begin(), f[m].end());
        if (ans == -inf) {
            std::cout << "No Solution" << endl;
        } else {
            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution_7637820722732645::solve();
    return 0;
}
