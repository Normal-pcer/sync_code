#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>
#include <string>
#include <vector>

auto main() -> int {
    std::vector a{0, 3, 2, 4, 3, 5, 4};

    std::mt19937 rng{std::random_device{}()};
    while (not std::ranges::is_sorted(a)) std::ranges::shuffle(a, rng);

    std::cout << (a
        | std::views::transform([](auto x) { return std::to_string(x); })
        | std::views::join_with(' ')
        | std::ranges::to<std::string>()) << '\n';
}
