/**
 * @link https://www.luogu.com.cn/problem/P8280
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_2789635989059777 {
    auto solve() -> void {
        
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T; std::cin >> T;
    for (auto t = T; t --> 0; ) {
        Solution_2789635989059777::solve();
    }
    return 0;
}
