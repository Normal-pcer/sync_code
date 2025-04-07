/**
 * @link
 */
#include "../../lib"
#include "../../libs/fixed_int.hpp"
using namespace lib;

#define FILENAME ""

namespace Solution_1003255212752722 {
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_1003255212752722::solve();
    return 0;
}
