/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "dollar"

namespace Solution_7637820722732645 {
    using f64 = double;
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<f64> a(n + 1);
        for (i32 i = 1; i <= n; i++) {
            std::cin >> a[i];
            a[i] /= 100;
        }

        // F[i] 表示
        // 考虑前 i 天。最后一天时持有美元。此时最多的钱。
        std::vector<f64> f(n + 1);

        f[0] = 100;
        for (i32 i = 1; i <= n; i++) {
            // 枚举最后一段持有马克的时间，即 [prev, i - 1]
            for (auto prev = i - 1; prev >= 1; prev--) {
                auto dollar = f[prev - 1];
                auto mark = dollar * a[prev];
                auto newDollar = mark / a[i];
                chkMax(f[i], newDollar);
            }
            chkMax(f[i], f[i - 1]);
        }
        std::cout << std::fixed << std::setprecision(2) << f[n] << endl;
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
