#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>

int constexpr n = 2e8;
std::array<unsigned int, n> a, b;

auto f() -> void {
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }
}

auto g() -> void {
    std::memcpy(b.data(), a.data(), sizeof(a));
}

auto main() -> int {
    std::mt19937 rng{std::random_device{}()};

    for (int i = 0; i < n; i++) {
        a[i] = rng();
    }

    auto t0 = std::chrono::system_clock::now();
    f();
    auto t1 = std::chrono::system_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
    return 0;
}