#include <bits/stdc++.h>
#include "libs/fixed_int.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N; std::cin >> N;
    std::string s; std::cin >> s;
    std::vector<i32> a(N);
    for (auto &x: a) std::cin >> x;

    i64 ans = 0;
    for (i32 i = 0; i < N; i += 2) {
        auto cur = 0;
        cur = std::max(cur, a[i + 1] - a[i]);
        if (i + 2 < N)  cur = std::max(cur, a[i + 2] - a[i]);
        ans += cur;
    }

    std::cout << ans << '\n';
}