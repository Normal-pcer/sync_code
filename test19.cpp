#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"

i32 constexpr n = 1 << 28;
std::array<u32, n> a;

auto f() -> u32 {
    u32 ans = 0;
    for (i32 i = 0; i < n; i++) {
        if (a[i] & 1) ans++;
    }
    return ans;
}

auto g() -> u32 {
    u32 ans = 0;
    for (i32 i = 0; i < n; i++) {
        ans += a[i] & 1;
    }
    return ans;
}

template <typename Func>
auto benchmark(std::string const &name, Func &&func, i32 warmup = 5, i32 run = 100) -> void {
    for (i32 i = 0; i < warmup; i++) {
        f();
    }

    i64 nanosec = 0;
    volatile u32 ans = 0;
    for (i32 i = 0; i < run; i++) {
        auto start = std::chrono::system_clock::now();
        ans += f();
        auto end = std::chrono::system_clock::now();
        nanosec += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    auto avg = static_cast<double>(nanosec) * 1e-6;
    std::cout << std::format("Benchmark `{}`: time = {:.3f}ms, ans = {}", name, avg, ans) << std::endl;
}

int main() {
    std::mt19937 rng{std::random_device{}()};

    for (i32 i = 0; i < n; i++) {
        a[i] = rng();
    }

    benchmark("f()", f);
    benchmark("g()", g);
    return 0;
}
