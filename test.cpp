#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>

int constexpr n = 2e8;
std::array<unsigned int, n> a, b;

auto f() -> unsigned {
    unsigned ans = 0;
    for (int i = 0; i + 1 < n; i += 64) {
        ans += a[i] * b[i + 1];
    }
    return ans;
}

auto main() -> int {
    std::mt19937 rng{std::random_device{}()};

    for (int i = 0; i < n; i++) {
        a[i] = rng();
    }

    auto t0 = std::chrono::system_clock::now();
    // std::memcpy(b.data(), a.data(), sizeof(a));
    auto ans = f();
    auto t1 = std::chrono::system_clock::now();

    std::cout << "ans = " << ans << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
    return 0;
}