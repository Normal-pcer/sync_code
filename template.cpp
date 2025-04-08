/**
 * @link
 */
#include "lib"
using namespace lib;

namespace Solution_/*hashName*/ {
    auto solve() -> void {

    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_/*hashName*/::solve();
    return 0;
}
