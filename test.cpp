#include <bits/stdc++.h>

#include "./libs/fixed_int.hpp"

int main() {
    i32 n, x, y, z;
    std::cin >> n >> x >> y >> z;
    std::vector<std::pair<i32, i32>> v(n);
    for (auto &[x, y]: v) std::cin >> x >> y;
    
    auto check = [&](i32 t) -> i32 {
        i32 res = 0;
        for (auto [a, b]: v) {
            if (t < a) res += x;
            else if (t > b) res += z;
            else res += y;
        }
        return res;
    };

    std::pair<i32, i32> max{};
    for (i32 t = -1; t < 10000; t++) {
        max = std::max(max, {check(t), t});
    }
    std::cout << max.first << "(t = " << max.second << ")\n";
    return 0;
}
