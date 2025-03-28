/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3285
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4467697901705253 {
    auto solve() -> void {
        i64 N; std::cin >> N;

        // 计算 N / i 之和
        i64 first_i = 1;
        i64 ans = 0;
        while (first_i <= N) {
            auto val = N / first_i;
            auto last_i = N / val + 1;
            auto cur = val * (last_i - first_i);
            first_i = last_i;
            ans += cur;
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_4467697901705253::solve();
    return 0;
}
