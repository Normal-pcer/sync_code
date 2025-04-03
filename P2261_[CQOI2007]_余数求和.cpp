/**
 * @link https://www.luogu.com.cn/problem/P2261
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6380689164048868 {
    auto solve() -> void {
        i32 N, K; std::cin >> N >> K;
        i64 ans = 0;
        for (i32 first_i = 1; first_i <= N; ) {
            auto val = K / first_i;
            auto last_i = val == 0? N + 1: K / val + 1;
            last_i = std::min(last_i, N + 1);

            auto cnt = last_i - first_i;
            auto scale = static_cast<i64>(first_i + first_i + cnt - 1) * cnt / 2;
            ans += scale * val;
            first_i = last_i;
        }

        ans = static_cast<i64>(N) * K - ans;
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_6380689164048868::solve();
    return 0;
}
