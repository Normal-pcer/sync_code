/**
 * @link https://www.luogu.com.cn/problem/P2260
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/*
考虑如下的子问题：

*/
namespace Solution_7648567164118096 {
    auto constexpr mulMod(i32 a, i32 b, i32 const mod) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    auto solve() -> void {
        i32 N, M;
        std::cin >> N >> M;

        i32 constexpr mod = 19'940'417;

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_7648567164118096::solve();
    return 0;
}
