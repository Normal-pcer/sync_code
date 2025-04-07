#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "T1"

namespace Solution_1003255212752722 {
    auto solve() -> void {
        i32 a, b, x, y;
        std::cin >> a >> b >> x >> y;

        i32 ans = 0x3f3f3f3f;
        if (a == 0 and b == 0) {
            chkMin(ans, 0);
        }
        if (a == 0 or b == 0) {
            chkMin(ans, y);
        }
        if (a == b) {
            chkMin(ans, x);
        }
        chkMin(ans, x + y);
        chkMin(ans, y + y);
        std::cout << ans << endl;
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
