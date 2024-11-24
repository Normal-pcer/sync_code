#include <bits/stdc++.h>

auto main() -> int {
    std::set<int> s {1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr const int x = 5;
    auto check = [&s](int x) -> void {
        if (s.contains(x))  std::print("Yes, there is {}!", x);
        else  std::print("No, this is not {}!", x);
    };
    check(x);
    return 0;
}