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
    Solution_/*hashName*/::solve();
    return 0;
}
