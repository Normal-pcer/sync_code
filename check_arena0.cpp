#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

#include "./libs/fixed_int.hpp"

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    i32 n, x, y; std::cin >> n >> x >> y;
    std::vector<i32> a(n);
    for (auto &x: a) std::cin >> x;

    auto min_max = [&](i32 l, i32 r) {
        auto [p, q] = std::minmax_element(a.begin() + l, a.begin() + r + 1);
        return std::pair(*p, *q);
    };

    auto check = [&](i32 l, i32 r) {
        auto [min, max] = min_max(l, r);
        return min == y && max == x;
    };

    i32 ans = 0;
    for (i32 i = 0; i < n; ++i) {
        for (i32 j = i; j < n; ++j) {
            ans += i32(check(i, j));
        }
    }

    std::cout << ans << '\n';
}

