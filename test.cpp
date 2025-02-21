// 二分判断

#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"


auto constexpr f(i32 x) -> i32 {
    if (x >= 10000) {
        if (x >= 1000000) {
            if (x >= 10000000)  return 7;
            else  return 6;
        } else {
            if (x >= 100000)  return 5;
            else  return 4;
        }
    } else {
        if (x >= 100) {
            if (x >= 1000)  return 3;
            else  return 2;
        } else {
            if (x >= 10)  return 1;
            else  return 0;
        }
    }
    return 0;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N;  std::cin >> N;
    i32 ans = 0;
    while (N--) {
        i32 x;  std::cin >> x;
        ans ^= f(x);
    }
    std::cout << ans << '\n';
    return 0;
}