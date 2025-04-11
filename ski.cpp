/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "ski"

namespace Solution_7637820722732645 {
    using f64 = double;
    struct Frac {
        i32 nume = 0, deno = 1;

        auto friend operator<= (Frac lhs, Frac rhs) -> bool {
            return static_cast<i64>(lhs.nume) * rhs.deno <= static_cast<i64>(rhs.nume) * lhs.deno;
        }
    };
    auto solve() -> void {
        i32 n, k;
        std::cin >> n >> k;

        std::vector<i32> h(n + 1);
        for (i32 i = 1; i <= n; i++) {
            std::cin >> h[i];
        }

        // f[i] 表示从 1 点开始，在 i 点停留，最小的支架数量。
        i32 constexpr inf = 0x3f3f3f3f;
        std::vector<i32> f(n + 1, inf);
        f[1] = 1;

        for (i32 i = 2; i <= n; i++) {
            // 如果斜率最小，则合法
            Frac minSlope{inf, 1};

            for (i32 j = i - 1; j >= 1; j--) {
                if (i - j > k) continue;

                Frac slope{h[i] - h[j], i - j};
                if (slope <= minSlope) {
                    minSlope = slope;
                    chkMin(f[i], f[j] + 1);
                }
            }
        }

        auto ans = f[n];
        for (i32 i = 1; i <= n; i++) {
            std::cout << "f[" << i << "] = " << f[i] << endl;
        }
        std::cout << ans << endl;
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
