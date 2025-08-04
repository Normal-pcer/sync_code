#include <algorithm>
#include <iostream>
#include <vector>

auto getLis(int n) -> std::vector<int>;

auto out(auto x) -> void { std::cout << x; }

auto main() -> int {
    auto lis = getLis(10);
    std::ranges::sort(lis);
    for (const auto& i : lis) {
        out(i);
    }
    out('\n');
}
