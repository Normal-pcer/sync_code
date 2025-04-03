#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
char constexpr endl = '\n';
#include "./libs/fixed_int.hpp"

#define FILENAME "wpn"

/*

*/
namespace Solution_6065476225470371 {
    auto solve() -> void {
        
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_6065476225470371::solve();
    return 0;
}
