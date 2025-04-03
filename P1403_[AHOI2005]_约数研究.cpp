/**
 * @link https://www.luogu.com.cn/problem/P1403
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3694989103032368 {
    auto solve() -> void {
        i32 N; std::cin >> N;
        i64 ans = 0;
        for (i32 first_i = 1; first_i <= N; ) {
            auto val = N / first_i;
            auto last_i = N / val + 1;
            ans += static_cast<i64>(last_i - first_i) * val;
            first_i = last_i;
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_3694989103032368::solve();
    return 0;
}
