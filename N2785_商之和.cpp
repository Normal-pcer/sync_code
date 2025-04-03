/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=2785
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4807554184580801 {
    auto solve() -> void {
        i64 N; std::cin >> N;

        i64 constexpr mod = 23'333'333'333'333'333;
        i64 ans = 0;
        for (i64 first_i = 1; first_i <= N; ) {
            auto q = N / first_i;
            auto last_i = N / q + 1;
            ans += q * (last_i - first_i), first_i = last_i;
            if (ans >= mod) ans -= mod;
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_4807554184580801::solve();
    return 0;
}
