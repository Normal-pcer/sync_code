#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "game"

namespace Solution_6686914644768679 {
    i32 constexpr mod = 1000000007;

    auto constexpr powMod(i64 a, i64 b) -> i32 {
        i64 ans = 1;
        for (; b != 0; b >>= 1, a = a * a % mod) {
            if (b & 1) ans = ans * a % mod;
        }
        return static_cast<i32>(ans);
    }
    auto solve() -> void {
        i32 n;
        std::cin >> n;
        std::cout << (static_cast<i64>(n) * powMod(2, n - 1)) % mod << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution_6686914644768679::solve();
    return 0;
}
